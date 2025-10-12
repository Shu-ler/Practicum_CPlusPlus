#include "stat_reader.h"
#include <iomanip>
#include <algorithm>

namespace stat_r {

	void ParseAndPrintStat(const trans_cat::TransportCatalogue& transport_catalogue, std::string_view request,
		std::ostream& output) {
		// ��������� ������ �� �������� �������� � �������
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
		const std::string_view& obj_name, std::ostream& output) {

		// ���� ������� � ������������ �����������
		auto route = transport_catalogue.FindRoute(obj_name);
		if (route) {

			// �������� ���������� �� ��������
			trans_cat::RouteStatistics stat = transport_catalogue.GetStat(route);

			// ������� ���������� � ������ �������
			output << "Bus " << obj_name << ": "
				<< stat.total_stops << " stops on route, "
				<< stat.unique_stops << " unique stops, "
				<< std::fixed << std::setprecision(2) << stat.route_length << " route length" << std::endl;
		}
		else {

			// ���� ������� �� ������
			output << "Bus " << obj_name << ": not found" << std::endl;
		}
	}

	void ProcessStopRequest(const trans_cat::TransportCatalogue& transport_catalogue,
		const std::string_view& obj_name, std::ostream& output) {
		auto stop = transport_catalogue.FindStop(obj_name);
		if (stop) {
			// ��������� �������
			auto& routes_set = transport_catalogue.GetRoutesByStop(stop);

			if (routes_set.empty()) {
				output << "Stop " << obj_name << ": no buses" << std::endl;
			}
			else {
				// �������� �������� ��������� � ������
				std::vector<std::string> route_names;
				for (auto& route : routes_set) {
					route_names.push_back(route->name_);
				}

				// ��������� ������ � ���������� ���������
				std::sort(route_names.begin(), route_names.end());

				// ������� ��������������� �������� ���������
				output << "Stop " << obj_name << ": buses ";

				for (const auto& route_name : route_names) {
					output << route_name;
					if (route_name != route_names.back()) {
						output << " ";
					}
				}
				output << std::endl;
			}
		}
		else {
			// ��������� �� �������
			output << "Stop " << obj_name << ": not found" << std::endl;
		}
	}

} // namespace stat_r
