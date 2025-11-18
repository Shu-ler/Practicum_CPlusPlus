#include <cmath>

#include "map_renderer.h"
#include <algorithm>

using namespace renderer;

renderer::MapRenderer::MapRenderer(const RenderSettings& settings)
    : settings_(settings) {
}

svg::Document MapRenderer::RenderMap(const trans_cat::TransportCatalogue& catalogue) const {
    svg::Document doc;

    std::set<const trans_cat::Stop*> stop_set;
    std::set<std::string> route_names;

    for (const auto& route : catalogue.GetRoutesSortedByName()) {
        route_names.insert(route->name);
        for (const auto* stop : route->stops) {
            stop_set.insert(stop);
        }
    }

    if (stop_set.empty()) {
        return doc;
    }

    // === Сортировка остановок по имени (лексикографически) ===
    std::vector<const trans_cat::Stop*> sorted_stops(stop_set.begin(), stop_set.end());
    std::sort(sorted_stops.begin(), sorted_stops.end(),
        [](const trans_cat::Stop* lhs, const trans_cat::Stop* rhs) {
            return lhs->name < rhs->name;
        });

    // === Проекция ===
    std::vector<geo::Coordinates> coords;
    for (const auto* stop : sorted_stops) {
        coords.push_back(stop->coordinates);
    }

    // Создаём проектор
    SphereProjector proj(coords.begin(), coords.end(), settings_.width, settings_.height, settings_.padding);

    // Маршруты
    std::vector<const trans_cat::Route*> routes;
    for (const auto& name : route_names) {
        if (const auto* route = catalogue.FindRoute(name); route && !route->stops.empty()) {
            routes.push_back(route);
        }
    }

    const auto& palette = settings_.color_palette.empty()
        ? std::vector<svg::Color>{svg::Color{ "black" }}
    : settings_.color_palette;

    // Рендерим всё

    // === 1. Рисуем линии маршрутов ===
    RenderRouteLines(doc, routes, palette, proj);

    // === 2. Рисуем подписи маршрутов ===
    RenderBusLabels(doc, routes, palette, proj);

    // === 3. Рисуем остановки ===
    RenderStops(doc, sorted_stops, proj);

    // === 4. Рисуем названия остановок ===
    RenderStopLabels(doc, sorted_stops, proj);

    return doc;
}

void MapRenderer::RenderRouteLines(svg::Document& doc,
    const std::vector<const trans_cat::Route*>& routes,
    const std::vector<svg::Color>& colors,
    const SphereProjector& proj) const {
    size_t color_idx = 0;
    for (const auto* route : routes) {

        if (route->stops.empty()) {
            continue;  // не рисуем
        }

        svg::Polyline line;

        // Прямой путь
        for (const auto* stop : route->stops) {
            line.AddPoint(proj(stop->coordinates));
        }

        // Если маршрут некольцевой — добавляем обратный (без первой)
        if (!route->is_roundtrip) {
            for (size_t i = route->stops.size() - 1; i-- > 0; ) {
                line.AddPoint(proj(route->stops[i]->coordinates));
            }
        }

        line
            .SetFillColor("none")
            .SetStrokeColor(colors[color_idx % colors.size()])
            .SetStrokeWidth(settings_.line_width)
            .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
            .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
        doc.Add(line);
        
        // Цвет сдвигается только если маршрут имеет остановки
        if (!route->stops.empty()) {
            ++color_idx;
        }
    }
}

void MapRenderer::RenderBusLabels(svg::Document& doc,
    const std::vector<const trans_cat::Route*>& routes,
    const std::vector<svg::Color>& colors,
    const SphereProjector& proj) const {
    size_t color_idx = 0;

    for (const auto* route : routes) {
        if (route->stops.empty()) continue;

        // Зацикливаем color - для случая, когда остановок больше, чем цветов
        const svg::Color& color = colors[color_idx % colors.size()];

        // Лямбда для добавления подписи в указанной позиции
        // Стоит в цикле (по [&] многое прилетает)
        auto add_label = [&](const trans_cat::Stop* stop) {
            const auto pos = proj(stop->coordinates);

            // Подложка — только обводка
            svg::Text under;
            under
                .SetPosition(pos)
                .SetOffset(settings_.bus_label_offset)
                .SetFontSize(settings_.bus_label_font_size)
                .SetFontFamily("Verdana")
                .SetFontWeight("bold")
                .SetFillColor(settings_.underlayer_color)
                .SetStrokeColor(settings_.underlayer_color)
                .SetStrokeWidth(settings_.underlayer_width)
                .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
                .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
                .SetData(route->name);
            doc.Add(under);

            // Основная надпись — только заливка
            svg::Text label;
            label
                .SetPosition(pos)
                .SetOffset(settings_.bus_label_offset)
                .SetFontSize(settings_.bus_label_font_size)
                .SetFontFamily("Verdana")
                .SetFontWeight("bold")
                .SetFillColor(color)
                .SetData(route->name);
            doc.Add(label);
            };

        // Выводим у первой остановки (всегда)
        add_label(route->stops.front());

        // Если маршрут некольцевой и есть хотя бы две разные остановки
        if (!route->is_roundtrip && route->stops.front() != route->stops.back()) {
            add_label(route->stops.back());
        }
        
        // Инкремент индекса цвета
        ++color_idx;
    }
}

void MapRenderer::RenderStops(svg::Document& doc,
    const std::vector<const trans_cat::Stop*>& stops,
    const SphereProjector& proj) const {
    for (const auto* stop : stops) {
        doc.Add(svg::Circle{}
            .SetCenter(proj(stop->coordinates))
            .SetRadius(settings_.stop_radius)
            .SetFillColor("white"));
    }
}

void MapRenderer::RenderStopLabels(svg::Document& doc,
    const std::vector<const trans_cat::Stop*>& stops,
    const SphereProjector& proj) const {
    for (const auto* stop : stops) {
        const auto pos = proj(stop->coordinates);

        // Подложка
        svg::Text under;
        under
            .SetPosition(pos)
            .SetOffset(settings_.stop_label_offset)
            .SetFontSize(settings_.stop_label_font_size)
            .SetFontFamily("Verdana")
            .SetFillColor(settings_.underlayer_color)
            .SetStrokeColor(settings_.underlayer_color)
            .SetStrokeWidth(settings_.underlayer_width)
            .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
            .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
            .SetData(stop->name);
        doc.Add(under);

        // Текст
        svg::Text label;
        label
            .SetPosition(pos)
            .SetOffset(settings_.stop_label_offset)
            .SetFontSize(settings_.stop_label_font_size)
            .SetFontFamily("Verdana")
            .SetFillColor("black")
            .SetData(stop->name);
        doc.Add(label);
    }
}