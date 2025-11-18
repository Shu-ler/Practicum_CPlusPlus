#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <regex>

#include "geo.h"
#include "transport_catalogue.h"

/*
 * Структура CommandDescription - хранение данных разобранной команды
 */
struct CommandDescription {
	// Определяет, задана ли команда (поле command непустое)
	explicit operator bool() const {
		return !command.empty();
	}

	bool operator!() const {
		return !operator bool();
	}

	std::string command{};		// Название команды
	std::string id{};			// id маршрута или остановки
	std::string description{};	// Параметры команды
	std::string distances{};	// Для команд Stop - расстояния до соседних остановок
};

/*
 * Структура StopData - передача данных остановки из функции парсинга команды добавления остановки
 * в методы добавления остановки в справочник и другие функции, использующие данные парсинга
 */
struct StopData {
	Coordinates coordinates{ 0.0, 0.0 };					// Географические координаты остановки
	std::unordered_map<std::string, int> nearby_stops{};	// Расстояния до соседних остановок
};

/*
 * Класс InputReader - отвечает за чтение и обработку входных данных для транспортного справочника
 */
class InputReader {
public:

	/*
	 * Regex для парсинга команд
	 * 
	 * Для команды Stop:
	 * ^ Stop([^ \s:] + (? : [^ \s:] *)*) : (.*)$
	 *		[^ \s:] + (? : [^ \s:] *) * — id может содержать пробелы, но не включает ':'
	 *		(.*) — всё остальное — description (координаты и расстояния до соседних остановок)
	 *
	 * Для команды Bus:
	 * ^ Bus([^ \s:] + ) : (.*)$
	 *		[^ \s:] + (? : [^ \s:] *) * — id может содержать пробелы, но не включает ':'
	 *		(.*) — всё остальное — description (маршрут)
	 */
	inline static const std::vector<std::pair<std::regex, std::string>> cmd_regs{
		std::pair(std::regex(R"(^Stop ([^\s:]+(?: [^\s:]*)*): (.*)$)"), "Stop"),
		std::pair(std::regex(R"(^Bus ([^\s:]+(?: [^\s:]*)*): (.*)$)"), "Bus") };

	/*
	 * Regex для координат. Извлекает широту, долготу и опциональную оставшуюся часть строки
	 *	([-+] ? \d * \. ? \d + ) — широта(с поддержкой знака, десятичной точки).
	 *	, \s* — запятая с возможными пробелами.
	 *	([-+] ? \d * \. ? \d + ) — долгота.
	 *	(? : , \s* (.*)) ? — опциональная часть после запятой(расстояния до соседних остановок).
	 */
	inline static const std::regex coords_regex{ R"(([-+]?\d*\.?\d+),\s*([-+]?\d*\.?\d+)(?:,\s*(.*))?)"	};

	// Regex для расстояний. Извлекает расстояние и имя остановки
	inline static const std::regex distance_regex{R"((\d+)m to ([^,]+))"};

	/*
	 * Заполнение транспортного справочника из указанного потока
	 */
	void LoadCatalog(std::istream& input, trans_cat::TransportCatalogue& catalogue);

	/*
	 * Выполнение запросов к транспортному справочнику из указанного потока и вывод в указанный поток
	 */
	void ProcessRequest(std::istream& input, std::ostream& output, trans_cat::TransportCatalogue& catalogue);

private:
	/*
	 * Парсит строку в структуру CommandDescription и сохраняет результат в commands_
	 */
	void ParseLine(std::string_view line);

	/*
	 * Наполняет данными транспортный справочник, используя команды из commands_
	 */
	void ApplyCommands(trans_cat::TransportCatalogue& catalogue) const;

private:
	// Контейнер хранения данных (структур) разобранных команд
	std::vector<CommandDescription> commands_;
};
