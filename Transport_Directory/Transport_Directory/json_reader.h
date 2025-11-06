#pragma once

#include "transport_catalogue.h"
#include "json.h"

/*
 * Структура библиотеки работы с JSON-входом:
 * 
 *  json_reader/
 *      ├── LoadFromJson   → читает base_requests
 *      ├── GetStatRequests → читает stat_requests
 *      └── MakeResponse   → формирует ответ
 * 
 */

namespace json_reader {

    /**
     * @brief Загружает данные транспортного справочника из JSON-документа.
     *
     * Парсит массив base_requests и:
     * - добавляет остановки (тип "Stop")
     * - добавляет маршруты (тип "Bus")
     * - устанавливает расстояния между остановками
     *
     * @param tc Ссылка на транспортный каталог, который будет заполнен данными
     * @param input JSON-документ, содержащий ключи "base_requests" и "stat_requests"
     *
     * @pre input.GetRoot() должен быть словарём (Dict)
     * @throw json::ParsingError если формат JSON нарушен
     *
     * @note Функция не очищает каталог перед загрузкой — новые данные добавляются к существующим.
     * @note Поддерживает как кольцевые (is_roundtrip: true), так и линейные маршруты.
     */
    void LoadFromJson(trans_cat::TransportCatalogue& tc, const json::Document& input);

    /**
     * @brief Извлекает массив статистических запросов из входного документа.
     * @param input Полный входной JSON-документ
     * @return Константная ссылка на массив запросов
     * @throws json::ParsingError если ключ 'stat_requests' отсутствует
     *
     * Используется для обработки запросов типа "Bus", "Stop".
     */
    const json::Array& GetStatRequests(const json::Document& input);

    /**
     * @brief Формирует JSON-ответы на массив запросов статистики.
     *
     * Для каждого запроса из stat_requests:
     * - если тип "Bus" — возвращает статистику маршрута
     * - если тип "Stop" — возвращает список автобусов, проходящих через остановку
     * - если сущность не найдена — возвращает сообщение об ошибке
     *
     * @param tc Константная ссылка на транспортный каталог
     * @param stat_requests Массив JSON-объектов с запросами (каждый имеет ключи "id", "type")
     * @return json::Document, содержащий массив ответов
     *
     * @note Порядок ответов совпадает с порядком запросов.
     * @note Все ответы содержат ключ "request_id", равный "id" из запроса.
     *
     * Пример ответа:
     * [
     *   { "request_id": 1, "stop_count": 5, "unique_stop_count": 3, ... },
     *   { "request_id": 2, "buses": ["750", "256"] },
     *   { "request_id": 3, "error_message": "not found" }
     * ]
     */
    json::Document MakeResponse(const trans_cat::TransportCatalogue& tc,
        const json::Array& stat_requests);

} // namespace json_reader
