#include "json_reader.h"
#include <stdexcept>
#include <string>

namespace {

    /**
     * @brief Извлекает значение по ключу из JSON-словаря с проверкой типа.
     *
     * Шаблонная функция, которая ищет ключ в словаре и пытается извлечь значение
     * заданного типа T с помощью Node::As<T>(). Если ключ не найден или тип не совпадает —
     * выбрасывает исключение ParsingError.
     *
     * @tparam T Тип ожидаемого значения (например, std::string, int, bool и т.д.)
     * @param dict JSON-словарь (Dict), в котором выполняется поиск
     * @param key  Имя ключа (std::string_view)
     * @return     Константная ссылка на значение типа T
     * @throws     json::ParsingError, если ключ отсутствует или тип не совпадает
     *
     * @note Используется внутри json_reader для безопасного извлечения полей
     *       из входных JSON-объектов. Предназначена только для внутреннего использования.
     */
    template<typename T>
    T GetJsonValue(const json::Dict& dict, std::string_view key) {
        auto it = dict.find(std::string(key));
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

    /**
     * @brief Начинает построение JSON-ответа, добавляя ключ "request_id".
     *
     * Эта вспомогательная функция используется для единообразного формирования
     * ответов на запросы: она начинает словарь и сразу добавляет поле "request_id",
     * взятое из входного запроса.
     *
     * @param builder Ссылка на Builder, который используется для построения ответа
     * @param req     Исходный JSON-объект запроса, содержащий ключ "id"
     * @return        Ссылка на тот же builder (для цепочки вызовов)
     *
     * @note Функция предполагает, что ключ "id" существует и имеет тип int.
     *       Используется внутри MakeResponse для устранения дублирования кода.
     */
    json::Builder& StartResponse(json::Builder& builder, const json::Dict& req) {
        int id = req.at("id").AsInt();
        return builder.StartDict().Key("request_id").AddNumber(id);
    }

    /**
     * @brief Создаёт ответ на запрос "Bus"
     * @param tc Транспортный каталог
     * @param request JSON-объект запроса (содержит "id", "name")
     * @return JSON-объект с результатом
     */
    json::Node MakeBusResponse(const trans_cat::TransportCatalogue& tc, const json::Dict& request) {
        json::Builder builder;

        int id = request.at("id").AsInt();
        std::string bus_name = request.at("name").AsString();
        auto stat = tc.GetRouteStat(bus_name);

        builder
            .StartDict()
            .Key("request_id").AddNumber(id);
            
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

        int id = request.at("id").AsInt();
        std::string stop_name = request.at("name").AsString();
        auto buses = tc.GetBusesByStop(stop_name);

        json::Builder buses_builder;
        buses_builder.StartArray();
        for (const auto& bus : buses) {
            buses_builder.AddString(bus);
        }
        buses_builder.EndArray();
        auto buses_node = buses_builder.Build();

        builder
            .StartDict()
            .Key("request_id").AddNumber(id)
            .Key("buses").AddNode(buses_node)
            .EndDict();

        return builder.Build();
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

        const json::Array& stops_array = route_node.at("stops").AsArray();
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
            std::string type = req.at("type").AsString();

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
                    json::Builder builder;
                    StartResponse(builder, req);

                    builder.Key("error_message").AddString("unknown request type");
                    responses.push_back(builder.EndDict().Build());
                }
            }
            catch (const json::ParsingError& e) {
                json::Builder builder;
                StartResponse(builder, req);
                builder.Key("error_message").AddString(std::string("parse error: ") + e.what());
                responses.push_back(builder.EndDict().Build());
            }
            catch (const std::exception& e) {
                json::Builder builder;
                StartResponse(builder, req);
                builder.Key("error_message").AddString(std::string("request error: ") + e.what());
                responses.push_back(builder.EndDict().Build());
            }
            catch (...) {
                json::Builder builder;
                StartResponse(builder, req);
                builder.Key("error_message").AddString("internal error");
                responses.push_back(builder.EndDict().Build());
            }
        }

        return json::Document(std::move(responses));
    }
} // namespace json_reader
