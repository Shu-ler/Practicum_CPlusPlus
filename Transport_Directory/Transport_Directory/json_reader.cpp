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
        if (root.count("stat_requests")) {
            return root.at("stat_requests").AsArray();
        }
        return {};
    }

    renderer::RenderSettings GetRenderSettings(const json::Document& input) {
        const auto& root = input.GetRoot().AsMap();
        const auto& rs = root.at("render_settings").AsMap();

        // Парсим color_palette
        std::vector<svg::Color> color_palette;
        const auto& palette_node = rs.at("color_palette");
        if (!palette_node.IsArray()) {
            throw json::ParsingError("color_palette must be an array");
        }
        for (const auto& color_node : palette_node.AsArray()) {
            color_palette.push_back(svg::ReadColor(color_node));
        }

        return renderer::RenderSettings{
            .width = rs.at("width").AsDouble(),
            .height = rs.at("height").AsDouble(),
            .padding = rs.at("padding").AsDouble(),
            .line_width = rs.at("line_width").AsDouble(),
            .stop_radius = rs.at("stop_radius").AsDouble(),
            .bus_label_font_size = rs.at("bus_label_font_size").AsInt(),
            .bus_label_offset = svg::Point{
                rs.at("bus_label_offset").AsArray()[0].AsDouble(),
                rs.at("bus_label_offset").AsArray()[1].AsDouble()
            },
            .stop_label_font_size = rs.at("stop_label_font_size").AsInt(),
            .stop_label_offset = svg::Point{
                rs.at("stop_label_offset").AsArray()[0].AsDouble(),
                rs.at("stop_label_offset").AsArray()[1].AsDouble()
            },
            .underlayer_color = svg::ReadColor(rs.at("underlayer_color")),
            .underlayer_width = rs.at("underlayer_width").AsDouble(),
            .color_palette = std::move(color_palette)
        };
    }

} // namespace json_reader
