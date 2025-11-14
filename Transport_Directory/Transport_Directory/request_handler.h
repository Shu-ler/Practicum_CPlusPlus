#pragma once

#include "transport_catalogue.h"
#include "json.h"
#include "map_renderer.h"

#include <optional>
#include <string>
#include <map>
#include <functional>

/**
 * @file request_handler.h
 * @brief Модуль обработки запросов к транспортному справочнику.
 *
 * Содержит класс RequestHandler — фасад, упрощающий взаимодействие между
 * JSON-парсером и подсистемами приложения (каталог, визуализация, маршрутизация).
 *
 * Позволяет:
 * - Получать статистику по автобусным маршрутам
 * - Получать список маршрутов, проходящих через остановку
 * - Генерировать карту маршрутов (если заданы render_settings)
 *
 * Поддерживает расширение функциональности без изменения интерфейса парсинга.
 */

namespace request_handler {

    /**
     * @brief Структура с метриками маршрута.
     */
    struct RouteStat {
        std::string name;               //< Название маршрута
        size_t stop_count = 0;          //< Общее количество остановок (с повторениями)
        size_t unique_stop_count = 0;   //< Количество уникальных остановок
        double route_length = 0.0;      //< Длина маршрута в метрах
        double curvature = 0.0;         //< Коэффициент извилистости (отношение route_length к геодезическому расстоянию)
    };

    /**
     * @brief Статистика по остановке.
     */
    struct StopStat {
        std::string name;                           ///< Название остановки
        std::vector<std::string> bus_names;         ///< Список маршрутов, проходящих через остановку
    };

    /**
     * @class RequestHandler
     * @brief Фасад для обработки запросов к транспортному справочнику.
     * 
     * См. паттерн проектирования Фасад: https://ru.wikipedia.org/wiki/Фасад_(шаблон_проектирования)
     *
     * Этот класс скрывает сложность взаимодействия с TransportCatalogue
     * и предоставляет простой интерфейс для получения данных.
     *
     * Позволяет:
     * - Получать статистику по маршрутам и остановкам
     * - Генерировать карту маршрутов (если заданы render_settings)
     *
     * Использует статический метод Create() для инициализации в зависимости от входных данных.
     */
    class RequestHandler {
    public:
        /**
         * @brief Фабричный метод: создаёт RequestHandler на основе входного JSON.
         * @param catalogue - заполненный транспортный каталог
         * @param input - входной JSON-документ
         * @return Настроенный RequestHandler
         */
        static RequestHandler Create(const trans_cat::TransportCatalogue& catalogue,
            const json::Document& input);

        /**
         * @brief Обрабатывает запросы и выводит результат в поток.
         * @param out Поток вывода (например, std::cout)
         */
        void ProcessRequests(std::ostream& out) const;

        /**
         * @brief Получает статистику по автобусному маршруту.
         * @param bus_name Название маршрута
         * @return Объект RouteStat, если маршрут найден; std::nullopt — если не найден
         */
        std::optional<RouteStat> GetBusStat(const std::string& bus_name) const;

        /**
         * @brief Получает статистику по остановке.
         * @param stop_name Название остановки
         * @return Объект StopStat, если остановка найдена; std::nullopt — если не найдена
         */
        std::optional<StopStat> GetStopStat(const std::string& stop_name) const;

    private:
        
        // Тип обработчика запроса: принимает json::Dict, возвращает json::Dict
        using HandlerFunc = std::function<json::Dict(const json::Dict&)>;

        /**
         * @brief Приватный конструктор.
         * @param catalogue Ссылка на транспортный каталог — источник данных
         */
        explicit RequestHandler(const trans_cat::TransportCatalogue& catalogue);

        RequestHandler(const trans_cat::TransportCatalogue& catalogue,
            std::optional<renderer::MapRenderer> renderer,
            std::optional<json::Array> stat_requests);

        // Приватные методы обработки
        json::Dict ProcessBusRequest(const json::Dict& req) const;
        json::Dict ProcessStopRequest(const json::Dict& req) const;
        json::Dict ProcessMapRequest(const json::Dict& req) const;
        json::Dict MakeErrorResponse(int id, std::string_view message) const;

        // Диспетчер команд: тип запроса → обработчик
        void InitializeHandlers();
        std::map<std::string, HandlerFunc> request_handlers_;

    private:
        const trans_cat::TransportCatalogue& catalogue_;
        std::optional<renderer::MapRenderer> map_renderer_;
        std::optional<json::Array> stat_requests_;
    };

} // namespace request_handler
