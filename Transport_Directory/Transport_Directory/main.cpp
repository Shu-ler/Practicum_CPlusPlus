#include "json_reader.h"
#include "transport_catalogue.h"
#include "json.h"

#include <iostream>
#include <exception>

/**
 * @brief Точка входа в приложение "Транспортный справочник".
 *
 * Приложение:
 * 1. Считывает JSON из stdin
 * 2. Заполняет транспортный каталог на основе "base_requests"
 * 3. Обрабатывает "stat_requests" и формирует ответ
 *    Выводит массив JSON-ответов в stdout
 *
 * Формат входа: объект с полями "base_requests" и "stat_requests"
 * Формат выхода: массив JSON-объектов с ответами на запросы
 *
 * @return 0 при успехе, 1 при ошибке
 */
int main() {

//#ifdef _DEBUG
//    if (freopen_s(nullptr, "input.json", "r", stdin) != 0) {
//        fprintf(stderr, "Cannot open input.json\n");
//        return 1;
//    }
//    if (freopen_s(nullptr, "output.json", "w", stdout) != 0) {
//        fprintf(stderr, "Cannot open output.json\n");
//        return 1;
//    }
//#endif

    try {
        // 1. Считываем и парсим входной JSON
        // Ожидается полный JSON-объект, содержащий все данные
        json::Document input;
        try {
            input = json::Load(std::cin);
        }
        catch (const json::ParsingError& e) {
            throw json::ParsingError("Failed to parse JSON: " + std::string(e.what()));
        }

//        json::Document input;
//        if (!(std::cin >> input)) {
//            throw json::ParsingError("Failed to parse JSON from input");
//        }

        // 2. Создаём и заполняем транспортный каталог
        // Используем данные из "base_requests" (остановки, маршруты, расстояния)
        trans_cat::TransportCatalogue catalogue;
        json_reader::LoadFromJson(catalogue, input);

        // 3. Обрабатываем запросы и выводим результат
        // Ответ формируется как массив JSON-объектов с полями "request_id" и данными
        std::cout << json_reader::MakeResponse(catalogue, 
            json_reader::GetStatRequests(input)) << std::endl;

        return 0;

    }
    catch (const json::ParsingError& e) {
        // Ошибка в формате входных данных — показываем сообщение как есть
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        // Внутренняя ошибка (логика, память и т.д.) — добавляем префикс
        std::cerr << "Internal error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        // Неожиданное исключение (крайний случай)
        std::cerr << "Unknown error occurred." << std::endl;
        return 1;
    }
}
