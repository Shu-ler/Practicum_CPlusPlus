#include "stat_reader.h"
#include <iomanip>

void ProcessBusRequest(const trans_cat::TransportCatalogue& tansport_catalogue, 
	const std::string_view& obj_name, std::ostream& output) {

	// Ищем маршрут в транспортном справочнике
	auto route = tansport_catalogue.FindRoute(obj_name);
	if (route) {
	
		// Получаем статистику по маршруту
		trans_cat::RouteStatistics stat = tansport_catalogue.GetStat(route);

		// Выводим статистику в нужном формате
		output << "Bus " << obj_name << ": "
			<< stat.total_stops << " stops on route, "
			<< stat.unique_stops << " unique stops, "
			<< std::fixed << std::setprecision(2) << stat.route_length << " route length" << std::endl;
	}
	else {
		
		// Если маршрут не найден
		output << "Bus " << obj_name << ": not found" << std::endl;
	}
}

void ProcessStopRequest(const trans_cat::TransportCatalogue& tansport_catalogue, 
	const std::string_view& obj_name, std::ostream& output) {
	auto stop = tansport_catalogue.FindStop(obj_name);
	if (stop) {
		// Остановка найдена
		auto& routes_set = tansport_catalogue.GetRoutesByStop(stop);

		if (routes_set.empty()) {
			output << "Stop " << obj_name << ": no buses" << std::endl;
		}
		else {
			output << "Stop " << obj_name << ": buses ";
			bool first_stop = true;
			for (auto& route : routes_set) {
				if (!first_stop) {
					output << " ";
				}
				output << route->name_;
				first_stop = false;
			}
		//	output << std::endl;
		}
	}
	else {
		// Остановка не найдена
		output << "Stop " << obj_name << ": not found" << std::endl;
	}
}

void ParseAndPrintStat(const trans_cat::TransportCatalogue& tansport_catalogue, std::string_view request,
	std::ostream& output) {
	// Разбиваем запрос на название маршрута и команду
	size_t pos = request.find(' ');
	std::string_view command = request.substr(0, pos);
	std::string_view obj_name = request.substr(pos + 1);

	if (command == "Bus") {
		ProcessBusRequest(tansport_catalogue, obj_name, output);
	}
	else if (command == "Stop") {
		ProcessStopRequest(tansport_catalogue, obj_name, output);
	}
	else {
		output << "Unknown command" << std::endl;
	}
}