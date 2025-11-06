#include "stat_reader.h"
#include <iomanip>
#include <algorithm>

namespace stat_r {

	void ParseAndPrintStat(const trans_cat::TransportCatalogue& transport_catalogue, std::string_view request,
		std::ostream& output) {

		// Разбиваем запрос на название маршрута и команду
		size_t pos = request.find(' ');
		std::string_view command = request.substr(0, pos);
		std::string_view obj_name = request.substr(pos + 1);

		if (command == "Bus") {
			ProcessBusRequest(transport_catalogue, obj_name, output);
		}
		else if (command == "Stop") {
			ProcessStopRequest(transport_catalogue, obj_name, output);
		}
		else {
			output << "Unknown command" << std::endl;
		}
	}

	void ProcessBusRequest(const trans_cat::TransportCatalogue& transport_catalogue,
		const std::string_view& route_name, std::ostream& output) {

		// Ищем маршрут в транспортном справочнике
		auto route = transport_catalogue.FindRoute(route_name);
		if (route) {

			// Получаем статистику по маршруту
			trans_cat::RouteStatistics stat = transport_catalogue.GetStat(route);

			// Выводим статистику в нужном формате
			output << "Bus " << route_name << ": "
				<< stat.total_stops << " stops on route, "
				<< stat.unique_stops << " unique stops, "
				<< std::fixed << std::setprecision(0) << stat.route_length << " route length, "
				<< std::fixed << std::setprecision(5) << stat.curvature << " curvature"
				<< std::endl;
		}
		else {

			// Если маршрут не найден
			output << "Bus " << route_name << ": not found" << std::endl;
		}
	}

	void ProcessStopRequest(const trans_cat::TransportCatalogue& transport_catalogue,
		const std::string_view& stop_name, std::ostream& output) {
		auto stop = transport_catalogue.FindStop(stop_name);
		if (stop) {
		
			// Остановка найдена
			auto& routes_names = transport_catalogue.GetRoutesNamesByStop(stop);

			if (routes_names.empty()) {
				output << "Stop " << stop_name << ": no buses" << std::endl;
			}
			else {

				// Выводим отсортированные названия маршрутов
				output << "Stop " << stop_name << ": buses ";

				auto it = routes_names.begin();
				for (; it != routes_names.end(); ++it) {
					output << *it;
					if (std::next(it) != routes_names.end()) {
						output << " ";
					}
				}
				output << std::endl;
			}
		}
		else {
			
			// Остановка не найдена
			output << "Stop " << stop_name << ": not found" << std::endl;
		}
	}

} // namespace stat_r
