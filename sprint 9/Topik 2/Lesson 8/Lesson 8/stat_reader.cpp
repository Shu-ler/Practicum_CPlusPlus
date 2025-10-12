#include "stat_reader.h"
#include <iomanip>

void ParseAndPrintStat(const trans_catalogue::TransportCatalogue& tansport_catalogue,
	std::string_view request,
	std::ostream& output) {
	// ��������� ������ �� �������� �������� � �������
	size_t pos = request.find(' ');
	std::string_view command = request.substr(0, pos);
	std::string_view route_name = request.substr(pos + 1);

	if (command == "Bus") {
		// ���� ������� � ������������ �����������
		RoutePtr route = tansport_catalogue.FindRoute(route_name);
		if (route) {
			// �������� ���������� �� ��������
			trans_catalogue::TransportCatalogue::RouteStatistics stat = tansport_catalogue.GetStat(route);

			// ������� ���������� � ������ �������
			output << "Bus " << route_name << ": "
				<< stat.total_stops << " stops on route, "
				<< stat.unique_stops << " unique stops, "
				<< std::fixed << std::setprecision(6) << stat.route_length << " route length" << std::endl;
		}
		else {
			// ���� ������� �� ������
			output << "Bus " << route_name << ": not found" << std::endl;
		}
	}
	else {
		output << "Unknown command" << std::endl;
	}
}

