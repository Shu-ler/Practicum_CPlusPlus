#include "json_reader.h"
#include "request_handler.h"

#include <iostream>
#include <exception>

int main() {
    try {
        // 1. Считываем JSON
        json::Document input = json::Load(std::cin);

        // 2. Создаём и заполняем транспортный каталог
        trans_cat::TransportCatalogue catalogue;
        json_reader::JSONReader reader(catalogue);
        reader.LoadFromJson(input);

        // 3. Создаём обработчик запросов
        request_handler::RequestHandler handler =
            request_handler::RequestHandler::Create(catalogue, input);

        // 4. Обрабатываем запросы и выводим результат
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
