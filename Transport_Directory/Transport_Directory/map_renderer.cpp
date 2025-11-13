#include <cmath>

#include "map_renderer.h"
#include <algorithm>

using namespace renderer;

// === Реализация SphereProjector ===

// === MapRenderer::Render — без изменений ===

renderer::MapRenderer::MapRenderer(const RenderSettings& settings)
    : settings_(settings) {
}

svg::Document MapRenderer::Render(const trans_cat::TransportCatalogue& catalogue) const {
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

    std::vector<geo::Coordinates> coords;
    for (const auto* stop : stop_set) {
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
  //  RenderBusLabels(doc, routes, palette, proj);

    // === 3. Рисуем остановки ===
  //  RenderStops(doc, std::vector<const trans_cat::Stop*>(stop_set.begin(), stop_set.end()), proj);

    // === 4. Рисуем названия остановок ===
  //  RenderStopLabels(doc, std::vector<const trans_cat::Stop*>(stop_set.begin(), stop_set.end()), proj);

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
        for (const auto* stop : route->stops) {
            line.AddPoint(proj(stop->coordinates));
        }

        line
            .SetFillColor("none")
            .SetStrokeColor(colors[color_idx % colors.size()])
            .SetStrokeWidth(settings_.line_width)
            .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
            .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
        doc.Add(line);
        
        // Цвет сдвигается только если маршрут **имеет остановки**
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

        const auto pos = proj(route->stops.front()->coordinates);

        // Подложка
        svg::Text under;
        under
            .SetPosition(pos)
            .SetOffset(settings_.bus_label_offset)
            .SetFontSize(settings_.bus_label_font_size)
            .SetFontFamily("Verdana")
            .SetFontWeight("bold")
            .SetData(route->name)
            .SetFillColor(settings_.underlayer_color)
            .SetStrokeColor(settings_.underlayer_color)
            .SetStrokeWidth(settings_.underlayer_width)
    //        .SetPaint(svg::PaintType::STROKE)
            ;
        doc.Add(under);

        // Текст
        svg::Text label;
        label
            .SetPosition(pos)
            .SetOffset(settings_.bus_label_offset)
            .SetFontSize(settings_.bus_label_font_size)
            .SetFontFamily("Verdana")
            .SetFontWeight("bold")
            .SetData(route->name)
            .SetFillColor(colors[color_idx % colors.size()]);
        doc.Add(label);

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
            .SetData(stop->name)
            .SetFillColor(settings_.underlayer_color)
            .SetStrokeColor(settings_.underlayer_color)
            .SetStrokeWidth(settings_.underlayer_width)
            //.SetPaint(svg::PaintType::STROKE)
            ;
        doc.Add(under);

        // Текст
        svg::Text label;
        label
            .SetPosition(pos)
            .SetOffset(settings_.stop_label_offset)
            .SetFontSize(settings_.stop_label_font_size)
            .SetFontFamily("Verdana")
            .SetData(stop->name)
            .SetFillColor("black");
        doc.Add(label);
    }
}