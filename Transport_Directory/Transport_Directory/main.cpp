#include "json_reader.h"
#include "request_handler.h"

#include <iostream>
#include <exception>

int main() {

    // 0. Настройка std::cout для чисел с плавающей точкой
    //std::cout.precision(6);
    //std::cout << std::fixed;

    try {
        // 1. Прочитать JSON
        json::Document input = json::Load(std::cin);

        // 2. Создать каталог
        trans_cat::TransportCatalogue catalogue;

        // 3. Создать ридер 
        json_reader::JSONReader reader(catalogue);

        // 4. Загрузить каталог
        reader.LoadFromJson(input);

        // 5. Создать обработчик запросов
        request_handler::RequestHandler handler = 
            request_handler::RequestHandler::Create(catalogue, input);

        // 6. Обработать и вывести результат
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
