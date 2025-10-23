#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <regex>

#include "geo.h"
#include "transport_catalogue.h"

/*
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

	std::string command{};		// Ќазвание команды
	std::string id{};			// id маршрута или остановки
	std::string description{};	// ѕараметры команды
	std::string distances{};	// ƒл€ команд Stop - рассто€ни€ до соседних остановок
};

/*
 *  ласс InputReader - отвечает за чтение и обработку входных данных дл€ транспортного справочника
 */
class InputReader {
public:

	/*
	 * Regex дл€ парсинга команд
	 * 
	 * ƒл€ команды Stop:
	 * ^ Stop([^ \s:] + (? : [^ \s:] *)*) : (.*)$
	 *		[^ \s:] + (? : [^ \s:] *) * Ч id может содержать пробелы, но не включает ':'
	 *		(.*) Ч всЄ остальное Ч description (координаты и рассто€ни€ до соседних остановок)
	 *
	 * ƒл€ команды Bus:
	 * ^ Bus([^ \s:] + ) : (.*)$
	 *		[^ \s:] + (? : [^ \s:] *) * Ч id может содержать пробелы, но не включает ':'
	 *		(.*) Ч всЄ остальное Ч description (маршрут)
	 */
	inline static const std::vector<std::pair<std::regex, std::string>> cmd_regs{
		std::pair(std::regex(R"(^Stop ([^\s:]+(?: [^\s:]*)*): (.*)$)"), "Stop"),
		std::pair(std::regex(R"(^Bus ([^\s:]+(?: [^\s:]*)*): (.*)$)"), "Bus") };

	/*
	 * Regex дл€ координат. »звлекает широту, долготу и опциональную оставшуюс€ часть строки
	 *	([-+] ? \d * \. ? \d + ) Ч широта(с поддержкой знака, дес€тичной точки).
	 *	, \s* Ч зап€та€ с возможными пробелами.
	 *	([-+] ? \d * \. ? \d + ) Ч долгота.
	 *	(? : , \s* (.*)) ? Ч опциональна€ часть после зап€той(рассто€ни€ до соседних остановок).
	 */
	inline static const std::regex coords_regex{ R"(([-+]?\d*\.?\d+),\s*([-+]?\d*\.?\d+)(?:,\s*(.*))?)"	};

	// Regex дл€ рассто€ний. »звлекает рассто€ние и им€ остановки
	inline static const std::regex distance_regex{R"((\d+)m to ([^,]+))"};

	/*
	 * «аполнение транспортного справочника из указанного потока
	 */
	void LoadCatalog(std::istream& input, trans_cat::TransportCatalogue& catalogue);

	/*
	 * ¬ыполнение запросов к транспортному справочнику из указанного потока и вывод в указанный поток
	 */
	void ProcessRequest(std::istream& input, std::ostream& output, trans_cat::TransportCatalogue& catalogue);

private:
	/*
	 * ѕарсит строку в структуру CommandDescription и сохран€ет результат в commands_
	 */
	void ParseLine(std::string_view line);

	/*
	 * Ќаполн€ет данными транспортный справочник, использу€ команды из commands_
	 */
	void ApplyCommands(trans_cat::TransportCatalogue& catalogue) const;

private:
	//  онтейнер хранени€ данных (структур) разобранных команд
	std::vector<CommandDescription> commands_;
};
