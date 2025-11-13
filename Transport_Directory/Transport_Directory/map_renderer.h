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

    inline const double EPSILON = 1e-6;
    inline bool IsZero(double value) {
        return std::abs(value) < EPSILON;
    }

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
        // points_begin и points_end задают начало и конец интервала элементов geo::Coordinates
        template <typename PointInputIt>
        SphereProjector(PointInputIt points_begin, PointInputIt points_end,
            double max_width, double max_height, double padding)
            : padding_(padding) //
        {
            // Если точки поверхности сферы не заданы, вычислять нечего
            if (points_begin == points_end) {
                return;
            }

            // Находим точки с минимальной и максимальной долготой
            const auto [left_it, right_it] = std::minmax_element(
                points_begin, points_end,
                [](auto lhs, auto rhs) { return lhs.lng < rhs.lng; });
            min_lon_ = left_it->lng;
            const double max_lon = right_it->lng;

            // Находим точки с минимальной и максимальной широтой
            const auto [bottom_it, top_it] = std::minmax_element(
                points_begin, points_end,
                [](auto lhs, auto rhs) { return lhs.lat < rhs.lat; });
            const double min_lat = bottom_it->lat;
            max_lat_ = top_it->lat;

            // Вычисляем коэффициент масштабирования вдоль координаты x
            std::optional<double> width_zoom;
            if (!IsZero(max_lon - min_lon_)) {
                width_zoom = (max_width - 2 * padding) / (max_lon - min_lon_);
            }

            // Вычисляем коэффициент масштабирования вдоль координаты y
            std::optional<double> height_zoom;
            if (!IsZero(max_lat_ - min_lat)) {
                height_zoom = (max_height - 2 * padding) / (max_lat_ - min_lat);
            }

            if (width_zoom && height_zoom) {
                // Коэффициенты масштабирования по ширине и высоте ненулевые,
                // берём минимальный из них
                zoom_coeff_ = std::min(*width_zoom, *height_zoom);
            }
            else if (width_zoom) {
                // Коэффициент масштабирования по ширине ненулевой, используем его
                zoom_coeff_ = *width_zoom;
            }
            else if (height_zoom) {
                // Коэффициент масштабирования по высоте ненулевой, используем его
                zoom_coeff_ = *height_zoom;
            }
        }

        // Проецирует широту и долготу в координаты внутри SVG-изображения
        svg::Point operator()(geo::Coordinates coords) const {
            return {
                (coords.lng - min_lon_) * zoom_coeff_ + padding_,
                (max_lat_ - coords.lat) * zoom_coeff_ + padding_
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
