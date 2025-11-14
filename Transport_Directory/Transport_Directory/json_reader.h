#pragma once

#include "transport_catalogue.h"
#include "json.h"
#include "map_renderer.h"

/*
 * Структура модуля json_reader:
 *
 * Модуль отвечает за:
 * - Загрузку данных транспортного справочника из JSON (базовые запросы)
 * - Извлечение статистических запросов
 * - Парсинг настроек визуализации карты
 *
 * Функции:
 *
 *   LoadFromJson()     → обрабатывает "base_requests": остановки, маршруты, расстояния
 *   GetStatRequests()  → извлекает массив "stat_requests": Bus, Stop, Map
 *   GetRenderSettings()→ парсит "render_settings" для визуализации карты
 *
 * Все функции потоконебезопасны.
 */

namespace json_reader {

    /**
     * @brief Загружает данные транспортного справочника из JSON-документа.
     *
     * Парсит массив "base_requests" и:
     * - добавляет остановки (тип "Stop")
     * - добавляет маршруты (тип "Bus")
     * - устанавливает дорожные расстояния между остановками
     *
     * @param tc Ссылка на транспортный каталог, который будет заполнен
     * @param input JSON-документ с ключом "base_requests"
     *
     * @pre input.GetRoot() должен быть словарём (Dict)
     * @throw json::ParsingError если формат JSON нарушен или отсутствуют обязательные поля
     *
     * @note Функция не очищает каталог перед загрузкой — данные добавляются.
     * @note Поддерживает кольцевые (`is_roundtrip: true`) и линейные маршруты.
     *
     * @example
     * {
     *   "base_requests": [
     *     { "type": "Stop", "name": "A", "latitude": 0.0, "longitude": 0.0 },
     *     { "type": "Bus", "name": "1", "stops": ["A", "B"], "is_roundtrip": false }
     *   ],
     *   "stat_requests": [...]
     * }
     */
    void LoadFromJson(trans_cat::TransportCatalogue& tc, const json::Document& input);

    /**
     * @brief Извлекает массив статистических запросов из входного документа.
     *
     * Возвращает массив запросов типа "Bus", "Stop", "Map" из ключа "stat_requests".
     * Если ключ отсутствует, возвращается пустой массив.
     *
     * @param input Полный входной JSON-документ
     * @return json::Array — копия массива статистических запросов
     *
     * @note Ранее функция возвращала ссылку, что приводило к ошибке возврата ссылки на временный объект.
     *       Теперь возвращается по значению — безопасно и соответствует стандартной практике.
     *
     * @example
     * [
     *   { "type": "Bus", "name": "1", "id": 1 },
     *   { "type": "Map", "id": 2 }
     * ]
     */
    const json::Array GetStatRequests(const json::Document& input);

    /**
     * @brief Парсит настройки визуализации карты из JSON.
     *
     * Читает словарь "render_settings" и заполняет структуру renderer::RenderSettings.
     * Если "render_settings" отсутствует, поведение не определено (вызов должен быть защищён проверкой).
     *
     * @param input Входной JSON-документ
     * @return Объект RenderSettings с настройками карты
     *
     * @throw json::ParsingError если формат "render_settings" нарушен
     *
     * @note Все значения должны присутствовать (кроме цвета underlayer_color, который может быть массивом или строкой).
     *
     * @example
     * "render_settings": {
     *   "width": 1000,
     *   "height": 500,
     *   "padding": 50,
     *   "stop_radius": 5,
     *   "line_width": 4,
     *   "bus_label_font_size": 20,
     *   "bus_label_offset": [7, 15],
     *   "stop_label_font_size": 16,
     *   "stop_label_offset": [7, -3],
     *   "underlayer_color": "white",
     *   "underlayer_width": 3.0,
     *   "color_palette": ["blue", "red", "green"]
     * }
     */
    renderer::RenderSettings GetRenderSettings(const json::Document& input);

} // namespace json_reader
