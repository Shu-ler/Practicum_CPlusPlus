#include "request_handler.h"
#include "json_reader.h"
#include "map_renderer.h"
#include <algorithm>

namespace request_handler {

    using namespace std::string_literals;

    // =============================================================================
    // Реализация  приватных конструкторов
    // =============================================================================

    RequestHandler::RequestHandler(const trans_cat::TransportCatalogue& catalogue)
        : catalogue_(catalogue)
        , map_renderer_(std::nullopt) {
    }

    RequestHandler::RequestHandler(const trans_cat::TransportCatalogue& catalogue,
        std::optional<renderer::MapRenderer> renderer,
        std::optional<json::Array> stat_requests)
        : catalogue_(catalogue)
        , map_renderer_(std::move(renderer))
        , stat_requests_(std::move(stat_requests)) {
    }

    RequestHandler RequestHandler::Create(const trans_cat::TransportCatalogue& catalogue, 
        const json::Document& input) {
        const auto& root = input.GetRoot().AsMap();

        // Проверяем, есть ли render_settings
        std::optional<renderer::MapRenderer> renderer = std::nullopt;
        if (root.count("render_settings")) {
            auto settings = json_reader::GetRenderSettings(input);
            renderer.emplace(settings);
        }

        std::optional<json::Array> stat_requests = json_reader::GetStatRequests(input);

        return RequestHandler(catalogue, std::move(renderer), std::move(stat_requests));
    }

    void RequestHandler::ProcessRequests(std::ostream& out) const {
        if (!stat_requests_) {
            throw std::logic_error("No stat_requests provided");
        }

        json::Array responses;
        responses.reserve(stat_requests_->size());

        for (const auto& req_node : *stat_requests_) {
            const auto& req = req_node.AsMap();
            int id = req.at("id").AsInt();
            std::string type = req.at("type").AsString();

            json::Builder builder;
            builder.StartDict().Key("request_id").AddNumber(id);

            if (type == "Bus") {
                auto stat = GetBusStat(req.at("name").AsString());
                if (stat) {
                    builder
                        .Key("stop_count").AddNumber(static_cast<int>(stat->stop_count))
                        .Key("unique_stop_count").AddNumber(static_cast<int>(stat->unique_stop_count))
                        .Key("route_length").AddNumber(static_cast<int>(stat->route_length))
                        .Key("curvature").AddNumber(stat->curvature);
                }
                else {
                    builder.Key("error_message").AddString("not found");
                }
            }
            else if (type == "Stop") {
                auto stat = GetStopStat(req.at("name").AsString());
                if (stat) {
                    json::Array buses;
                    for (const auto& bus : stat->bus_names) {
                        buses.push_back(json::Node(bus));
                    }
                    builder.Key("buses").AddNode(buses);
                }
                else {
                    builder.Key("error_message").AddString("not found");
                }
            }
            else if (type == "Map") {
                // Генерируем SVG и преобразуем в строку
                svg::Document doc = map_renderer_->RenderMap(catalogue_);
                std::ostringstream ss;
                doc.Render(ss);  // получаем строку SVG
                builder.Key("map").AddString(ss.str());
            }
            else {
                builder.Key("error_message").AddString("unknown request type");
            }

            responses.push_back(builder.EndDict().Build());
        }

        // Выводим массив JSON
        json::Print(json::Document(std::move(responses)), out);
        out << std::endl;
    }

    std::optional<RouteStat> RequestHandler::GetBusStat(const std::string& bus_name) const {
        auto stat = catalogue_.GetRouteStat(bus_name);
        if (!stat) {
            return std::nullopt;
        }

        RouteStat result;
        result.name = bus_name;
        result.stop_count = stat->stop_count;
        result.unique_stop_count = stat->unique_stop_count;
        result.route_length = stat->route_length;
        result.curvature = stat->curvature;

        return result;
    }

    std::optional<StopStat> RequestHandler::GetStopStat(const std::string& stop_name) const {
        const auto* stop = catalogue_.FindStop(stop_name);
        if (!stop) {
            return std::nullopt;
        }

        StopStat result;
        result.name = stop_name;
        const auto& buses = catalogue_.GetBusesByStop(stop_name);
        result.bus_names.assign(buses.begin(), buses.end());
        std::sort(result.bus_names.begin(), result.bus_names.end());

        return result;
    }

} // namespace request_handler