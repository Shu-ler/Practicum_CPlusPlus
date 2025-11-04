#include "json_reader.h"
#include "transport_catalogue.h"
#include "json.h"

#include <iostream>
#include <exception>

/**
 * @brief Точка входа в приложение "Транспортный справочник".
 *
 * Программа:
 * 1. Читает JSON-документ из стандартного ввода (stdin)
 * 2. Парсит базовые данные (остановки и маршруты) и заполняет каталог
 * 3. Обрабатывает запросы статистики
 * 4. Выводит массив JSON-ответов в стандартный вывод (stdout)
 *
 * Формат входных данных:
 * {
 *   "base_requests": [ ... ],
 *   "stat_requests": [ ... ]
 * }
 *
 * Формат выходных данных:
 * [
 *   { "request_id": 1, ... },
 *   { "request_id": 2, ... }
 * ]
 *
 * @return 0 при успешном выполнении, 1 — при ошибке
 */
int main() {
    try {
        // 1. Читаем весь JSON из stdin
        json::Document input = json::Load(std::cin);

        // 2. Создаём транспортный каталог и загружаем в него данные
        trans_cat::TransportCatalogue catalogue;
        json_reader::LoadFromJson(catalogue, input);

        // 3. Извлекаем массив запросов
        const auto& root = input.GetRoot().AsMap();
        auto it = root.find("stat_requests");
        if (it == root.end()) {
            throw std::runtime_error("Missing 'stat_requests' in input JSON");
        }
        const json::Array& stat_requests = it->second.AsArray();

        // 4. Формируем ответы
        json::Document response = json_reader::MakeResponse(catalogue, stat_requests);

        // 5. Выводим результат в stdout
        json::Print(response, std::cout);
        std::cout << std::endl;

        return 0;  // Успех

    }
    catch (const std::exception& e) {
        // Если произошла ошибка — выводим в stderr и завершаемся с кодом 1
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
        return 1;
    }
}
