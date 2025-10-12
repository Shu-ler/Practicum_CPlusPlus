#include "transport_catalogue.h"
#include <unordered_set>
#include <optional>
#include <utility>

namespace route {
	
	Route::Route(const std::string& name, const RouteStops& stops) :
		name_(name),
		stops_(stops) {
	}

	const RouteStops& Route::GetStops() const {
		return stops_;
	}

	void Route::SetStops(const RouteStops& stops) {
		stops_ = stops;
	}

	void Route::AddStop(StopPtr stop) {
		stops_.push_back(stop);
	}

	bool Route::IsRingRoute(const std::string& route) {
		return route.find('>') != std::string::npos;
	}

	bool Route::IsOrdinaryRoute(const std::string& route)	{
		return route.find('-') != std::string::npos;
	}

} // namespace route

namespace trans_catalogue {

	void TransportCatalogue::AddStop(std::string_view name, Coordinates pos) {
		// ��������� ��������� � ��� ���������
		stops_.push_back({ name, pos });

		// �������� ��������� �� ����������� ���������
		auto added_ptr = stops_.back();

		// ��������� ������������� ������ ��� �������� ������� �� ����� ���������
		auto key = added_ptr.GetName();
		stop_by_name_[key] = &added_ptr;
	}

	void TransportCatalogue::AddRoute(std::string name, std::vector<StopPtr> stops) {
		// ��������� ������� � ������ ���������
		buses_.push_back({ std::move(name), std::move(stops) });

		// �������� ��������� �� ����������� �������
		RoutePtr added_ptr = &buses_.back();

		// ��������� ������������� ������ ��� �������� ������� �� ����� ��������
		bus_by_name_[added_ptr->GetName()] = added_ptr;
	}

	StopPtr TransportCatalogue::FindStop(std::string_view stop_name) const {
		auto iter = stop_by_name_.find(std::string(stop_name));
		return (iter == stop_by_name_.end()) ? nullptr : iter->second;
	}

	RoutePtr TransportCatalogue::FindRoute(std::string_view route_name) const {
		auto iter = bus_by_name_.find(route_name);
		return (iter == bus_by_name_.end()) ? nullptr : iter->second;
	}

	TransportCatalogue::RouteStatistics TransportCatalogue::GetStat(RoutePtr route) const {
		RouteStatistics stat{};
		
		// ������� ������ ���������� ���������
		stat.total_stops = route->GetStops().size();

		// ����������� ���������� ���������
		std::unordered_set<std::string_view> unique_stops;
		for (auto stop : route->GetStops()) {
			unique_stops.insert(stop->GetName());
		}
		stat.unique_stops = unique_stops.size();

		// ������ ����� ��������
		std::optional<Coordinates> prev_pos;
		for (auto stop : route->GetStops()) {
			if (prev_pos) {
				stat.route_length += ComputeDistance(*prev_pos, stop->GetCoordinates());
			}
			prev_pos = stop->GetCoordinates();
		}
		return RouteStatistics();
	}

	TransportCatalogue::StopsIndex TransportCatalogue::GetStopsIndex() {
		return stop_by_name_;
	}

	TransportCatalogue::StopsIndex TransportCatalogue::GetStopsIndex() const {
		return stop_by_name_;
	}

} // namespace trans_catalogue

