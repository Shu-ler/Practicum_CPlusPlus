#pragma once
#include <string>
#include <string_view>
#include <vector>

#include "geo.h"
#include "transport_catalogue.h"

struct CommandDescription {
    // ќпредел€ет, задана ли команда (поле command непустое)
    explicit operator bool() const {
        return !command.empty();
    }

    bool operator!() const {
        return !operator bool();
    }

    std::string command;      // Ќазвание команды
    std::string id;           // id маршрута или остановки
    std::string description;  // ѕараметры команды
};

class InputReader {
public:
    /**
     * «аполнение транспортного справочника из указанного потока
     */
    void LoadCatalog(std::istream& input, trans_cat::TransportCatalogue& catalogue);

    /**
     * ¬ыполнение запросов к транспортному справочнику из указанного потока и вывод в указанный поток
     */
    void ProcessRequest(std::istream& input, std::ostream& output, trans_cat::TransportCatalogue& catalogue);

private:
    /**
     * ѕарсит строку в структуру CommandDescription и сохран€ет результат в commands_
     */
    void ParseLine(std::string_view line);

    /**
     * Ќаполн€ет данными транспортный справочник, использу€ команды из commands_
     */
    void ApplyCommands(trans_cat::TransportCatalogue& catalogue) const;

private:
    std::vector<CommandDescription> commands_;
};
