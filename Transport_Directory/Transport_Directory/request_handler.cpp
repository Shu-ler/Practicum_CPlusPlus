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
        if (root.count("render_settings")) {
            auto settings = json_reader::GetRenderSettings(input);
            renderer::MapRenderer renderer(settings);
            return RequestHandler(catalogue, std::move(renderer), std::nullopt);
        }

        // Иначе — обработка stat_requests
        const auto& requests = json_reader::GetStatRequests(input);
        return RequestHandler(catalogue, std::nullopt, requests);
    }

    void RequestHandler::ProcessRequests(std::ostream& out) const {
        if (map_renderer_) {
            // Режим: SVG-карта
            svg::Document map = map_renderer_->Render(catalogue_);
            map.Render(out);  // ← напрямую выводим SVG в поток
            out << std::endl;
        }
        else if (stat_requests_) {
            // Режим: JSON-ответы
            json::Array responses;
            responses.reserve(stat_requests_->size());

            for (const auto& req_node : *stat_requests_) {
                const json::Dict& req = req_node.AsMap();
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
                        builder.Key("buses").StartArray();
                        for (const auto& bus : stat->bus_names) {
                            builder.AddString(bus);
                        }
                        builder.EndArray();
                    }
                    else {
                        builder.Key("error_message").AddString("not found");
                    }
                }
                else {
                    builder.Key("error_message").AddString("unknown request type");
                }

                responses.push_back(builder.EndDict().Build());
            }

            json::Print(json::Document(std::move(responses)), out);
            out << std::endl;
        }
        else {
            throw std::logic_error("RequestHandler has no data to process");
        }
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