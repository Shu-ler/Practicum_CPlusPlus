#pragma once
#include <string>
#include <string_view>
#include <vector>

#include "geo.h"
#include "transport_catalogue.h"

using StopPtr = const stop::Stop*;
using RoutePtr = const route::Route*;
using RouteStops = std::vector<StopPtr>;

struct CommandDescription {
    // Определяет, задана ли команда (поле command непустое)
    explicit operator bool() const {
        return !command.empty();
    }

    bool operator!() const {
        return !operator bool();
    }

    std::string command;      // Название команды
    std::string id;           // id маршрута или остановки
    std::string description;  // Параметры команды
};

//bool operator<(const CommandDescription& lhs, const CommandDescription& rhs) {
//    return lhs.command < rhs.command; // Сортировка по полю command
//}
//
//bool operator<(const CommandDescription& lhs, const std::string& rhs) {
//    return lhs.command < rhs; // Сравнение поля command со строкой
//}

class InputReader {
public:
    /**
     * Парсит строку в структуру CommandDescription и сохраняет результат в commands_
     */
    void ParseLine(std::string_view line);

    /**
     * Наполняет данными транспортный справочник, используя команды из commands_
     */
    void ApplyCommands(trans_catalogue::TransportCatalogue& catalogue);

private:
    std::vector<CommandDescription> commands_;
};
