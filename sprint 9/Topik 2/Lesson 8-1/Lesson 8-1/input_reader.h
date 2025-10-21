#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <regex>

#include "geo.h"
#include "transport_catalogue.h"

/**
 * —труктура CommandDescription - хранение данных разобранной команды
 */
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

/**
 *  ласс InputReader - отвечает за чтение и обработку входных данных дл€ транспортного справочника
 */
class InputReader {
public:

	// Ўаблоны регул€рных выражений дл€ парсинга команд
	inline static const std::vector<std::pair<std::regex, std::string>> cmd_regs{
		std::pair(std::regex(R"(^Stop ([^\s:]+(?: [^\s:]*)*): (.*)$)"), "Stop"),
		std::pair(std::regex(R"(^Bus ([^\s:]+): (.*)$)"), "Bus") };

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
	//  онтейнер хранени€ данных (структур) разобранных команд
	std::vector<CommandDescription> commands_;
};
