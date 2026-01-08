#include "request_handler.h"
#include "json_reader.h"
#include "json_builder.h"
#include <sstream>

namespace request_handler {

	using namespace std::string_literals;

	RequestHandler::RequestHandler(const trans_cat::TransportCatalogue& catalogue,
								std::optional<renderer::MapRenderer> renderer,
								std::optional<json::Array> stat_requests,
								json_reader::RoutingSettings routing_settings)
		: catalogue_(catalogue)
		, map_renderer_(std::move(renderer))
		, stat_requests_(std::move(stat_requests))
		, routing_settings_(routing_settings) 
	{
		processor_.AddHandler("Bus", [this](const json::Dict& req) { return ProcessBusRequest(req); });
		processor_.AddHandler("Stop", [this](const json::Dict& req) { return ProcessStopRequest(req); });
		processor_.AddHandler("Map", [this](const json::Dict& req) { return ProcessMapRequest(req); });
		processor_.AddHandler("Route", [this](const json::Dict& req) { return ProcessRouteRequest(req); });
	}

	json::Dict RequestHandler::ProcessBusRequest(const json::Dict& req) const {
		auto stat = GetBusStat(req.at("name").AsString());
		int id = req.at("id").AsInt();

		if (!stat) {
			return MakeErrorResponse(id, "not found");
		}

		return json::Builder{}
			.StartDict()
			.Key("request_id").Value(id)
			.Key("stop_count").Value(static_cast<int>(stat->stop_count))
			.Key("unique_stop_count").Value(static_cast<int>(stat->unique_stop_count))
			.Key("route_length").Value(stat->route_length)
			.Key("curvature").Value(stat->curvature)
			.EndDict()
			.Build().AsDict();
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
			.Key("request_id").Value(id)
			.Key("buses").Value(std::move(buses))
			.EndDict()
			.Build().AsDict();
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
			.Key("request_id").Value(id)
			.Key("map").Value(ss.str())
			.EndDict()
			.Build().AsDict();
	}

	json::Dict RequestHandler::ProcessRouteRequest(const json::Dict& req) const {
		int id = req.at("id").AsInt();
		std::string from = req.at("from").AsString();
		std::string to = req.at("to").AsString();

		// Пока просто возвращаем заглушку
		return json::Builder{}
			.StartDict()
			.Key("request_id").Value(id)
			.Key("total_time").Value(0.0)
			.EndDict()
			.Build().AsDict();
	}

	json::Dict RequestHandler::MakeErrorResponse(int id, std::string_view message) {
		return json::Builder{}
			.StartDict()
			.Key("request_id").Value(id)
			.Key("error_message").Value(std::string{ message })
			.EndDict()
			.Build().AsDict();
	}

	RequestHandler RequestHandler::Create(const trans_cat::TransportCatalogue& catalogue,
		const json::Document& input) {
		const auto& root = input.GetRoot().AsDict();

		std::optional<renderer::MapRenderer> renderer = std::nullopt;
		if (root.count("render_settings")) {
			auto settings = json_reader::JSONReader::GetRenderSettings(input);
			renderer.emplace(settings);
		}

		std::optional<json::Array> stat_requests = json_reader::JSONReader::GetStatRequests(input);

		json_reader::RoutingSettings routing_settings = json_reader::JSONReader::GetRoutingSettings(input);

		return RequestHandler(catalogue, std::move(renderer), std::move(stat_requests), routing_settings);
	}

	void RequestHandler::ProcessRequests(std::ostream& out) const {
		if (!stat_requests_) {
			throw std::logic_error("No stat_requests provided");
		}

		auto responses = processor_.Process(*stat_requests_);
		processor_.Print(responses, out);
	}

	std::optional<RouteStat> RequestHandler::GetBusStat(const std::string& bus_name) const {
		auto stat = catalogue_.GetRouteStat(bus_name);
		if (!stat) {
			return std::nullopt;
		}

		return RouteStat{
			bus_name,
			stat->stop_count,
			stat->unique_stop_count,
			stat->route_length,
			stat->curvature
		};
	}

	std::optional<StopStat> RequestHandler::GetStopStat(const std::string& stop_name) const {
		const trans_cat::Stop* stop = catalogue_.FindStop(stop_name);
		if (!stop) {
			return std::nullopt;
		}

		StopStat result;
		result.name = stop_name;

		const auto& routes = catalogue_.GetBusesByStop(stop);
		for (const trans_cat::Route* route : routes) {
			result.bus_names.push_back(route->name);
		}
		return result;
	}

} // namespace request_handler
