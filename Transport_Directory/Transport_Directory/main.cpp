#include "json_reader.h"
#include "request_handler.h"

#include <iostream>
#include <exception>

int main() {
    try {
        // Читаем весь входной JSON
        json::Document input = json::Load(std::cin);

        // Создаём каталог
        trans_cat::TransportCatalogue catalogue;

        // Загружаем базовые данные (остановки и маршруты)
        json_reader::JSONReader reader(catalogue);
        reader.LoadFromJson(input);

        // Создаём обработчик запросов
        auto handler = request_handler::RequestHandler::Create(catalogue, input);

        // Обрабатываем статистические запросы и выводим ответ
        handler.ProcessRequests(std::cout);

        return 0;
    }
    catch (const json::ParsingError& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown error" << std::endl;
        return 1;
    }
}
