#pragma once

#include <iosfwd>
#include <string_view>

#include "transport_catalogue.h"

namespace stat_r {

	// Обрабатывает запрос для автобуса и выводит информацию о нём
	void ProcessBusRequest(const trans_cat::TransportCatalogue& transport_catalogue,
		const std::string_view& obj_name, std::ostream& output);

	// Обрабатывает запрос для остановки и выводит информацию о ней
	void ProcessStopRequest(const trans_cat::TransportCatalogue& transport_catalogue,
		const std::string_view& obj_name, std::ostream& output);

	// Парсит запрос и выводит статистику на основе транспортного справочника
	void ParseAndPrintStat(const trans_cat::TransportCatalogue& transport_catalogue,
		std::string_view request, std::ostream& output);

} // namespace stat_r