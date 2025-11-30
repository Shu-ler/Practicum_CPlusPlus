#include "json_builder.h"
#include "main.h"

#include <iostream>
#include <string_view>

using namespace std::literals;

// =============================================================================
// Прототипы тестов
// =============================================================================

void TestStringJSON();
void TestArrayJSON();
void TestDictJSON();
void TestGeneralJSON();

void PrintTestDescription(std::string_view msg);

// =============================================================================
// Главная функция
// =============================================================================

int main() {
    try {
        TestStringJSON();
        TestArrayJSON();
        TestDictJSON();
        TestGeneralJSON();
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << '\n';
        return 1;
    }
    catch (...) {
        std::cerr << "Неизвестная ошибка\n";
        return 1;
    }

    return 0;
}

// =============================================================================
// Вспомогательные функции
// =============================================================================

void PrintTestDescription(std::string_view msg) {
    std::cout << "\n\n"
        << "============================\n"
        << msg << '\n'
        << "============================\n";
}

// =============================================================================
// Тесты
// =============================================================================

void TestStringJSON() {
    PrintTestDescription("String JSON");
    json::Print(
        json::Document{
            json::Builder{}
                .Value("just a string"s)
                .Build()
        },
        std::cout
    );
}

void TestArrayJSON() {
    PrintTestDescription("Array JSON");
    json::Print(
        json::Document{
            json::Builder{}
                .StartArray()
                    .Value(1)
                    .Value(2)
                    .Value(3)
                .EndArray()
                .Build()
        },
        std::cout
    );
}

void TestDictJSON() {
    PrintTestDescription("Dict JSON");
    json::Print(
        json::Document{
            json::Builder{}
                .StartDict()
                    .Key("1"s).StartDict().EndDict()
                    .Key("2"s).Value(2)
                .EndDict()
                .Build()
        },
        std::cout
    );
}

void TestGeneralJSON() {
    PrintTestDescription("General JSON");
    json::Print(
        json::Document{
            json::Builder{}
                .StartDict()
                    .Key("key1"s).Value(123)
                    .Key("key2"s).Value("value2"s)
                    .Key("key3"s).StartArray()
                        .Value(456)
                        .StartDict().EndDict()
                        .StartDict()
                            .Key(""s).Value(nullptr)
                        .EndDict()
                        .Value(""s)
                    .EndArray()
                .EndDict()
                .Build()
        },
        std::cout
    );
}
