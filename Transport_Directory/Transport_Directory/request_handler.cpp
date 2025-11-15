#include "request_handler.h"
#include "json_reader.h"
#include "map_renderer.h"
#include <algorithm>

namespace request_handler {

    using namespace std::string_literals;

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
        InitializeHandlers();
    }

    json::Dict RequestHandler::ProcessBusRequest(const json::Dict& req) const {
        auto stat = GetBusStat(req.at("name").AsString());
        int id = req.at("id").AsInt();

        if (!stat) {
            return MakeErrorResponse(id, "not found");
        }

        return json::Builder{}
            .StartDict()
                .Key("request_id").AddNumber(id)
                .Key("stop_count").AddNumber(static_cast<int>(stat->stop_count))
                .Key("unique_stop_count").AddNumber(static_cast<int>(stat->unique_stop_count))
                .Key("route_length").AddNumber(static_cast<int>(stat->route_length))
                .Key("curvature").AddNumber(stat->curvature)
            .EndDict()
            .Build().AsMap();
    }

    json::Dict RequestHandler::ProcessStopRequest(const json::Dict& req) const {
        auto stat = GetStopStat(req.at("name").AsString());
        int id = req.at("id").AsInt();

        if (!stat) {
            return MakeErrorResponse(id, "not found");
        }

        json::Array buses;
        for (const auto& bus : stat->bus_names) {
            buses.push_back(json::Node(bus));
        }

        return json::Builder{}
            .StartDict()
                .Key("request_id").AddNumber(id)
                .Key("buses").AddNode(std::move(buses))
            .EndDict()
            .Build().AsMap();
    }

    json::Dict RequestHandler::ProcessMapRequest(const json::Dict& req) const {
        int id = req.at("id").AsInt();

        if (!map_renderer_) {
            return MakeErrorResponse(id, "render settings are not provided");
        }

        svg::Document doc = map_renderer_->RenderMap(catalogue_);
        std::ostringstream ss;
        doc.Render(ss);

        return json::Builder{}
            .StartDict()
                .Key("request_id").AddNumber(id)
                .Key("map").AddString(ss.str())
                .EndDict()
            .Build().AsMap();
    }

    json::Dict RequestHandler::MakeErrorResponse(int id, std::string_view message) const {
        return json::Builder{}
            .StartDict()
                .Key("request_id").AddNumber(id)
                .Key("error_message").AddString(std::string(message))
            .EndDict()
            .Build().AsMap();
    }

	void RequestHandler::InitializeHandlers() {
		request_handlers_["Bus"] = [this](const json::Dict& req) {
			return ProcessBusRequest(req);
			};
		request_handlers_["Stop"] = [this](const json::Dict& req) {
			return ProcessStopRequest(req);
			};
		request_handlers_["Map"] = [this](const json::Dict& req) {
			return ProcessMapRequest(req);
			};
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
            std::string type = req.at("type").AsString();

            auto it = request_handlers_.find(type);
            if (it != request_handlers_.end()) {
                responses.push_back(it->second(req));
            }
            else {
                responses.push_back(MakeErrorResponse(req.at("id").AsInt(), "unknown request type"));
            }
        }

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
        const trans_cat::Stop* stop = catalogue_.FindStop(stop_name);
        if (!stop) {
            return std::nullopt;
        }

        StopStat result;

        // Получаем ссылку на маршруты — O(1)
        const auto& routes = catalogue_.GetBusesByStop(stop);

        // Формируем имена — только один раз, при необходимости
        for (const trans_cat::Route* route : routes) {
            result.bus_names.push_back(route->name);
        }

        return result;
    }

} // namespace request_handler