#pragma once

#include "transport_catalogue.h"
#include "svg.h"

#include <vector>
#include <set>
#include <algorithm>

/**
 * @mainpage Модуль визуализации карты
 *
 * Структура модуля:
 *
 * map_renderer/
 *     ├── SphereProjector       → проецирует geo::Coordinates → svg::Point
 *     ├── RenderSettings        → хранит настройки из JSON
 *     ├── MapRenderer           → основной рендерер
 *     │   ├── RenderRouteLines  → рисует линии маршрутов
 *     │   ├── RenderBusLabels   → подписи автобусов
 *     │   ├── RenderStops       → кружки остановок
 *     │   └── RenderStopLabels  → подписи остановок
 *     └── (всё зависит от svg и domain)
 *
 * Зависимости:
 * - svg — для генерации SVG
 * - transport_catalogue — только от domain (Route, Stop, Coordinates)
 * - НЕ зависит от json, main, request_handler — легко переиспользуется
 */

namespace renderer {

    /**
     * @brief Проекция географических координат на прямоугольный SVG-холст.
     *
     * Преобразует широту и долготу в экранные координаты (x, y),
     * масштабируя точки так, чтобы они поместились в заданную область
     * с учётом отступов.
     *
     * Использует упрощённую цилиндрическую проекцию с поправкой на широту.
     */
    class SphereProjector {
    public:
        template <typename PointInputIt>
        SphereProjector(PointInputIt points_begin, PointInputIt points_end,
            double max_width, double max_height, double padding)
            : padding_(padding)
        {
            if (points_begin == points_end) return;

            const auto [min_lon_it, max_lon_it] = std::minmax_element(
                points_begin, points_end,
                [](const auto& a, const auto& b) { return a.lng < b.lng; });
            min_lon_ = min_lon_it->lng;
            const double max_lon = max_lon_it->lng;

            const auto [min_lat_it, max_lat_it] = std::minmax_element(
                points_begin, points_end,
                [](const auto& a, const auto& b) { return a.lat < b.lat; });
            const double min_lat = min_lat_it->lat;
            max_lat_ = max_lat_it->lat;

            const double width = (max_lon - min_lon_) * std::cos(max_lat_ * geo::PI / 180.0);
            const double height = max_lat_ - min_lat;

            const double width_scale = width * geo::DEG_TO_METER;
            const double height_scale = height * geo::DEG_TO_METER;

            zoom_coeff_ = std::min(
                (max_width - 2 * padding) / width_scale,
                (max_height - 2 * padding) / height_scale
            );
        }

        svg::Point operator()(geo::Coordinates coords) const {
            return {
                padding_ + (coords.lng - min_lon_) * zoom_coeff_ * geo::DEG_TO_METER,
                padding_ + (max_lat_ - coords.lat) * zoom_coeff_ * geo::DEG_TO_METER
            };
        }

    private:
        double padding_;
        double min_lon_ = 0;
        double max_lat_ = 0;
        double zoom_coeff_ = 0;
    };

    /**
    * @brief Настройки визуализации карты.
    *
    * Заполняются из JSON-поля "render_settings".
    * Определяют внешний вид карты: размеры, цвета, шрифты и т.д.
    */
    struct RenderSettings {
        double width;                   ///< Ширина SVG-изображения в пикселях
        double height;                  ///< Высота SVG-изображения
        double padding;                 ///< Отступ от краёв

        double line_width;              ///< Толщина линий маршрутов
        double stop_radius;             ///< Радиус окружности остановки

        int bus_label_font_size;        ///< Размер шрифта названий маршрутов
        svg::Point bus_label_offset;    ///< Смещение подписи маршрута от остановки

        int stop_label_font_size;       ///< Размер шрифта названий остановок
        svg::Point stop_label_offset;   ///< Смещение подписи остановки

        svg::Color underlayer_color;    ///< Цвет подложки под текстом
        double underlayer_width;        ///< Толщина подложки (stroke-width)

        std::vector<svg::Color> color_palette; ///< Цветовая палитра для маршрутов
    };

    /**
     * @class MapRenderer
     * @brief Основной класс для генерации SVG-карты маршрутов.
     *
     * Принимает настройки визуализации и использует данные из TransportCatalogue
     * для построения карты с маршрутами, остановками и подписями.
     *
     * @note Не зависит от JSON-парсера — только от RenderSettings и Catalogue.
     *       Это позволяет легко менять формат конфигурации (JSON → XML → TOML).
     */
    class MapRenderer {
    public:
        /**
         * @brief Конструктор.
         * @param settings Настройки визуализации
         */
        explicit MapRenderer(const RenderSettings& settings);

        /**
         * @brief Рендерит карту в SVG-документ.
         * @param catalogue Источник данных — транспортный справочник
         * @return Готовый SVG-документ с картой
         */
        svg::Document Render(const trans_cat::TransportCatalogue& catalogue) const;

    private:

        // Методы рендеринга
        void RenderRouteLines(svg::Document& doc,
            const std::vector<const trans_cat::Route*>& routes,
            const std::vector<svg::Color>& colors,
            const SphereProjector& proj) const;

        void RenderBusLabels(svg::Document& doc,
            const std::vector<const trans_cat::Route*>& routes,
            const std::vector<svg::Color>& colors,
            const SphereProjector& proj) const;

        void RenderStops(svg::Document& doc,
            const std::vector<const trans_cat::Stop*>& stops,
            const SphereProjector& proj) const;

        void RenderStopLabels(svg::Document& doc,
            const std::vector<const trans_cat::Stop*>& stops,
            const SphereProjector& proj) const;

    private:
        RenderSettings settings_;
    };

} // namespace renderer
