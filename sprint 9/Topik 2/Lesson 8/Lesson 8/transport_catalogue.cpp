#include "transport_catalogue.h"
#include <unordered_set>
#include <optional>
#include <utility>

namespace bus {
	
	Bus::Bus(const std::string& name, const RouteStops& stops) :
		name_(name),
		stops_(stops) {
	}

	const RouteStops& Bus::GetStops() const {
		return stops_;
	}

	void Bus::SetStops(const RouteStops& stops) {
		stops_ = stops;
	}

	void Bus::AddStop(StopPtr stop) {
		stops_.push_back(stop);
	}


	//RouteStat Bus::GetStat() const {
	//	RouteStat stat{};

	//	// ���������� ��������� ��� �������� ���������� ���������
	//	std::unordered_set<std::string_view> seen_stops{};
	//	std::optional<Coordinates> prev_pos{};

	//	for (auto stop : stops_) {

	//		// ����������� ����� ���������� ���������
	//		++stat.total_stops;

	//		// ���������, �������� �� ��������� ����������
	//		if (seen_stops.count(stop->name) == 0) {
	//			++stat.unique_stops; // ����������� ���������� ���������� ���������
	//			seen_stops.insert(stop->name);
	//		}

	//		// ���� ���� ���������� �������, ��������� ���������� ����� �����������
	//		if (prev_pos) {
	//			stat.route_length += ComputeDistance(*prev_pos, stop->position);
	//		}

	//		// ��������� ���������� �������
	//		prev_pos = stop->position;
	//	}

	//	return stat;
	//}

	bool Bus::IsRingRoute(const std::string& route) {
		return route.find('>') != std::string::npos;
	}

	bool Bus::IsOrdinaryRoute(const std::string& route)	{
		return route.find('-') != std::string::npos;
	}

} // namespace bus

namespace trans_catalogue {

	void TransportCatalogue::AddStop(std::string name, Coordinates pos) {
		// ��������� ��������� � ������ ���������
		stops_.push_back({ std::move(name), pos });

		// �������� ��������� �� ����������� ���������
		StopPtr added_ptr = &stops_.back();

		// ��������� ������������� ������ ��� �������� ������� �� ����� ���������
		stop_by_name_[added_ptr->GetName()] = added_ptr;
	}

	void TransportCatalogue::AddRoute(std::string name, std::vector<StopPtr> stops) {
		// ��������� ������� � ������ ���������
		buses_.push_back({ std::move(name), std::move(stops) });

		// �������� ��������� �� ����������� �������
		BusPtr added_ptr = &buses_.back();

		// ��������� ������������� ������ ��� �������� ������� �� ����� ��������
	//	bus_by_name_[added_ptr.] = added_ptr;
	}

	StopPtr TransportCatalogue::FindStop(std::string_view stop_name) const {
		auto iter = stop_by_name_.find(stop_name);
		return (iter == stop_by_name_.end()) ? nullptr : iter->second;
	}

	BusPtr TransportCatalogue::FindRoute(std::string_view route_name) const {
		auto iter = bus_by_name_.find(route_name);
		return (iter == bus_by_name_.end()) ? nullptr : iter->second;
	}

	TransportCatalogue::RouteStatistics TransportCatalogue::GetStat() const {
		return RouteStatistics();
	}

} // namespace trans_catalogue

