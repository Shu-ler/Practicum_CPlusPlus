#pragma once
#include <string>
#include <string_view>
#include <vector>

#include "geo.h"
#include "transport_catalogue.h"

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

class InputReader {
public:
    /**
     * Парсит строку в структуру CommandDescription и сохраняет результат в commands_
     */
    void ParseLine(std::string_view line);

    /**
     * Наполняет данными транспортный справочник, используя команды из commands_
     */
    void ApplyCommands(trans_cat::TransportCatalogue& catalogue) const;

    /**
     * Возвращает разделитель названий остановок
     * Для некорректной строки возвращает '*'
     */
    static char RouteStopDivider(const std::string_view description);

private:
    static bool IsOrdinaryRoute(const std::string_view description);
    static bool IsRingRoute(const std::string_view description);

private:
    std::vector<CommandDescription> commands_;
};
