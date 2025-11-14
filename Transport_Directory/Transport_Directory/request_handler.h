#pragma once

#include "transport_catalogue.h"
#include "json.h"
#include "map_renderer.h"

#include <optional>
#include <string>

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

        // ////////////////////////////////////////////////////////////////////////
        // Следующие методы будут добавлены в следующих частях проекта
        // ////////////////////////////////////////////////////////////////////////


         /**
          * @brief Рассчитывает маршрут от одной остановки до другой.
          * @param from Начальная остановка
          * @param to Конечная остановка
          * @return Описание маршрута или std::nullopt, если маршрут недостижим
          * @note Реализуется в части "Маршрутизация"
          */
          // std::optional<Router::RouteInfo> BuildRoute(const std::string& from, const std::string& to) const;

    private:
        /**
         * @brief Приватный конструктор.
         * @param catalogue Ссылка на транспортный каталог — источник данных
         */
        explicit RequestHandler(const trans_cat::TransportCatalogue& catalogue);

        RequestHandler(const trans_cat::TransportCatalogue& catalogue,
            std::optional<renderer::MapRenderer> renderer,
            std::optional<json::Array> stat_requests);

        const trans_cat::TransportCatalogue& catalogue_;
        std::optional<renderer::MapRenderer> map_renderer_;
        std::optional<json::Array> stat_requests_;
    };

} // namespace request_handler


// TODO удалить из релиза исходное описание
 // Класс RequestHandler играет роль Фасада, упрощающего взаимодействие JSON reader-а
 // с другими подсистемами приложения.
 // См. паттерн проектирования Фасад: https://ru.wikipedia.org/wiki/Фасад_(шаблон_проектирования)
 /*
 class RequestHandler {
 public:
     // MapRenderer понадобится в следующей части итогового проекта
     RequestHandler(const TransportCatalogue& db, const renderer::MapRenderer& renderer);

     // Возвращает информацию о маршруте (запрос Bus)
     std::optional<RouteStat> GetBusStat(const std::string_view& bus_name) const;

     // Возвращает маршруты, проходящие через
     const std::unordered_set<BusPtr>* GetBusesByStop(const std::string_view& stop_name) const;

     // Этот метод будет нужен в следующей части итогового проекта
     svg::Document RenderMap() const;

 private:
     // RequestHandler использует агрегацию объектов "Транспортный Справочник" и "Визуализатор Карты"
     const TransportCatalogue& db_;
     const renderer::MapRenderer& renderer_;
 };
 */
