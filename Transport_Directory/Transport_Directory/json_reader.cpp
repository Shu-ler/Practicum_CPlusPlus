#include "json_reader.h"
#include <stdexcept>
#include <string>

namespace json_reader {

    JSONReader::JSONReader(trans_cat::TransportCatalogue& tc)
        : catalogue_(tc) {
    }

    void JSONReader::LoadFromJson(const json::Document& input) {

        // Читаем root из json'а
        const json::Dict& root = input.GetRoot().AsDict();

        // Попытка найти в root узел 'base_requests'
        auto it = root.find("base_requests");
        if (it == root.end()) {
            throw json::ParsingError("Missing 'base_requests' in input");
        }

        // Получение узла (массива)
        const json::Array& base_requests = it->second.AsArray();

        // В цикле обрабатываем остановки и маршруты
        for (const auto& req_node : base_requests) {
            const json::Dict& req = req_node.AsDict();
            std::string type = req.at("type").AsString();

            if (type == "Stop") {
                AddStopFromJSON(req);
            }
            else if (type == "Bus") {
                AddRouteFromJSON(req);
            }
            else {
                throw json::ParsingError("Unknown base request type: " + type);
            }
        }
    }

    json::Array JSONReader::GetStatRequests(const json::Document& input) {
        const auto& root = input.GetRoot().AsDict();
        if (root.count("stat_requests")) {
            return root.at("stat_requests").AsArray();
        }
        return {};
    }

    renderer::RenderSettings JSONReader::GetRenderSettings(const json::Document& input) {
        
        // Читаем root из json'а
        const auto& root = input.GetRoot().AsDict();

        // Читаем словарь с настройками рендеринга
        const auto& rs = root.at("render_settings").AsDict();

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

    void JSONReader::AddStopFromJSON(const json::Dict& stop_node) {
        
        // Берем из узла json наименование и координаты
        std::string name = GetJsonValue<std::string>(stop_node, "name");
        double lat = GetJsonValue<double>(stop_node, "latitude");
        double lng = GetJsonValue<double>(stop_node, "longitude");

        // Добавляем или обновляем остановку в каталоге
        catalogue_.AddStop(name, { lat, lng });

        // Берем словарь расстояний
        auto dist_it = stop_node.find("road_distances");

        // При наличии - заносим расстояния в каталог
        if (dist_it != stop_node.end()) {
            const json::Dict& distances = dist_it->second.AsDict();
            for (const auto& [to, dist_node] : distances) {
                int distance = dist_node.AsInt();
                catalogue_.SetDistance(name, to, distance);
            }
        }
    }

    void JSONReader::AddRouteFromJSON(const json::Dict& route_node) {

        // Берем из узла json наименование и тип маршрута
        std::string name = GetJsonValue<std::string>(route_node, "name");
        bool is_roundtrip = GetJsonValue<bool>(route_node, "is_roundtrip");

        // Берем массив наименований остановок
        const json::Array& stops_array = route_node.at("stops").AsArray();

        // Готовим вектор под наименования
        std::vector<std::string> stops;
        stops.reserve(stops_array.size());

        // Заполняем значениями из узла (массива) json
        for (const auto& stop_node : stops_array) {
            stops.push_back(stop_node.AsString());
        }

        // Добавляем маршрут в каталог
        catalogue_.AddRoute(name, std::move(stops), is_roundtrip);

    }

} // namespace json_reader
