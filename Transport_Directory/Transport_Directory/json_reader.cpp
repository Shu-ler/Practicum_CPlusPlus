#include "json_reader.h"
#include <stdexcept>
#include <string>

namespace {

    // Вспомогательные функции для обработки отдельных типов запросов
    json::Node MakeBusResponse(const trans_cat::TransportCatalogue&, const json::Dict&);
    json::Node MakeStopResponse(const trans_cat::TransportCatalogue&, const json::Dict&);

    /**
     * @brief Создаёт ответ на запрос "Bus"
     * @param tc Транспортный каталог
     * @param request JSON-объект запроса (содержит "id", "name")
     * @return JSON-объект с результатом
     */
    json::Node MakeBusResponse(const trans_cat::TransportCatalogue& tc, const json::Dict& request) {
        json::Builder builder;
        builder.StartDict();

        int id = request.at("id").AsInt();
        builder.Key("request_id").AddValue(id);

        std::string bus_name = request.at("name").AsString();
        auto stat = tc.GetRouteStat(bus_name);

        if (stat) {
            builder
                .Key("stop_count").AddValue(static_cast<int>(stat->stop_count))
                .Key("unique_stop_count").AddValue(static_cast<int>(stat->unique_stop_count))
                .Key("route_length").AddValue(static_cast<int>(stat->route_length))
                .Key("curvature").AddValue(stat->curvature);
        }
        else {
            builder.Key("error_message").AddValue("not found");
        }

        return builder.EndDict().Build();
    }

    /**
     * @brief Создаёт ответ на запрос "Stop"
     * @param tc Транспортный каталог
     * @param request JSON-объект запроса
     * @return JSON-объект с результатом
     */
    json::Node MakeStopResponse(const trans_cat::TransportCatalogue& tc, const json::Dict& request) {
        json::Builder builder;
        builder.StartDict();

        int id = request.at("id").AsInt();
        builder.Key("request_id").AddValue(id);

        std::string stop_name = request.at("name").AsString();
        auto buses = tc.GetBusesByStop(stop_name);

        builder.Key("buses").StartArray();
        for (const auto& bus : buses) {
            builder.AddValue(bus);
        }
        builder.EndArray();

        return builder.EndDict().Build();
    }

    // Вспомогательная функция: проверяет, есть ли ключ и правильного ли он типа
    template<typename T>
    const T& GetJsonValue(const json::Dict& dict, std::string_view key) {
        auto it = dict.find(key);
        if (it == dict.end()) {
            throw json::ParsingError{ std::string("Key not found: ") + std::string(key) };
        }
        try {
            return it->second.As<T>();
        }
        catch (const std::bad_variant_access&) {
            throw json::ParsingError{ std::string("Key '") 
                + std::string(key) + "' has wrong type" };
        }
    }

    // Обработка остановки из base_requests
    void AddStopFromJson(trans_cat::TransportCatalogue& tc, const json::Dict& stop_node) {
        std::string name = GetJsonValue<std::string>(stop_node, "name");
        double lat = GetJsonValue<double>(stop_node, "latitude");
        double lng = GetJsonValue<double>(stop_node, "longitude");

        tc.AddStop(name, { lat, lng });

        auto dist_it = stop_node.find("road_distances");
        if (dist_it != stop_node.end()) {
            const json::Dict& distances = dist_it->second.AsMap();
            for (const auto& [to, dist_node] : distances) {
                int distance = dist_node.AsInt();
                tc.SetDistance(name, to, distance);
            }
        }
    }

    // Обработка маршрута из base_requests
    void AddRouteFromJson(trans_cat::TransportCatalogue& tc, const json::Dict& route_node) {
        std::string name = GetJsonValue<std::string>(route_node, "name");
        bool is_roundtrip = GetJsonValue<bool>(route_node, "is_roundtrip");

        const json::Array& stops_array = GetJsonValue<json::Array>(route_node, "stops");
        std::vector<std::string> stops;
        stops.reserve(stops_array.size());
        for (const auto& stop_node : stops_array) {
            stops.push_back(stop_node.AsString());
        }

        tc.AddRoute(name, std::move(stops), is_roundtrip);
    }

} // namespace anonymous

namespace json_reader {

    void LoadFromJson(trans_cat::TransportCatalogue& tc, const json::Document& input) {
        const json::Dict& root = input.GetRoot().AsMap();

        auto it = root.find("base_requests");
        if (it == root.end()) {
            throw json::ParsingError("Missing 'base_requests' in input");
        }
        const json::Array& base_requests = it->second.AsArray();

        for (const auto& req_node : base_requests) {
            const json::Dict& req = req_node.AsMap();
            std::string type = GetJsonValue<std::string>(req, "type");

            if (type == "Stop") {
                AddStopFromJson(tc, req);
            }
            else if (type == "Bus") {
                AddRouteFromJson(tc, req);
            }
            else {
                throw json::ParsingError("Unknown base request type: " + type);
            }
        }
    }

    const json::Array& GetStatRequests(const json::Document& input) {
        const auto& root = input.GetRoot().AsMap();
        auto it = root.find("stat_requests");
        if (it == root.end()) {
            throw json::ParsingError("Missing 'stat_requests' in input JSON");
        }
        return it->second.AsArray();
    }

    json::Document MakeResponse(const trans_cat::TransportCatalogue& tc, const json::Array& stat_requests) {
        json::Array responses;
        responses.reserve(stat_requests.size());

        for (const auto& req_node : stat_requests) {
            const json::Dict& req = req_node.AsMap();
            std::string type = GetJsonValue<std::string>(req, "type");

            try {
                if (type == "Bus") {
                    responses.push_back(MakeBusResponse(tc, req));
                }
                else if (type == "Stop") {
                    responses.push_back(MakeStopResponse(tc, req));
                }
                else {
                    // Неизвестный тип запроса
                    json::Builder builder;
                    builder.StartDict()
                        .Key("request_id").AddValue(GetJsonValue<int>(req, "id"))
                        .Key("error_message").AddValue("unknown request type");
                    responses.push_back(builder.EndDict().Build());
                }
            }
            catch (const std::exception&) {
                // Защита от падений при некорректных запросах
                json::Builder builder;
                builder.StartDict()
                    .Key("request_id").AddValue(GetJsonValue<int>(req, "id"))
                    .Key("error_message").AddValue("invalid request");
                responses.push_back(builder.EndDict().Build());
            }
        }

        return json::Document(std::move(responses));
    }
} // namespace json_reader
