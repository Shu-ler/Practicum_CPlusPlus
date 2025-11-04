#pragma once

#include "geo.h"
#include <string>
#include <vector>


/*
 * Структура проекта (по рекомендациям)
 *  transport-catalogue/
 *  ├── geo.h/cpp                   # координаты
 *  ├── domain.h                    # общие классы: Stop, Bus
 *  ├── transport_catalogue.h/cpp   # класс TransportCatalogue 
 *  ├── request_handler.h/cpp       # фасад для обработки запросов
 *  ├── json.h/cpp                  # библиотека JSON
 *  ├── json_reader.h/cpp           # парсинг JSON → заполнение каталога + формирование ответов
 *  └── main.cpp                    # основной модуль
 */

namespace trans_cat {

    /**
     * @brief Представляет остановку общественного транспорта.
     *
     * Остановка характеризуется названием и географическими координатами.
     * Используется как элемент маршрутов (Route) и как узел в системе расстояний.
     */
    struct Stop {
        std::string name;               ///< Название остановки (уникальное в рамках системы)
        geo::Coordinates coordinates;   ///< Широта и долгота остановки

        /**
         * @brief Сравнивает две остановки по имени.
         * @param other Другая остановка
         * @return true, если имена совпадают, иначе false
         *
         * @note Две остановки с одинаковым именем считаются одинаковыми,
         *       даже если координаты отличаются (согласно логике задачи).
         */
        bool operator==(const Stop& other) const;
    };

    /**
     * @brief Представляет маршрут общественного транспорта.
     *
     * Маршрут — это последовательность остановок, по которым ходит транспорт.
     * Может быть кольцевым (is_roundtrip = true) или линейным (с обратным путём).
     */
    struct Route {
        std::string name;               ///< Уникальное название маршрута
        std::vector<const Stop*> stops; ///< Остановки маршрута (указатели)
        bool is_roundtrip = false;      ///< Является ли маршрут кольцевым

        /**
         * @brief Проверяет, равны ли два маршрута по имени.
         * @param other Другой маршрут
         * @return true, если имена совпадают
         *
         * @note Сравнение идёт только по имени. Два маршрута с одинаковым
         *       именем — это один и тот же маршрут.
         */
        bool operator==(const Route& other) const;
    };

} // namespace trans_cat
