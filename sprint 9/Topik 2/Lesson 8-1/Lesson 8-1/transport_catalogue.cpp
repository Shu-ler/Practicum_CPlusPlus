#include "transport_catalogue.h"
#include <unordered_set>
#include <optional>

namespace trans_cat {

	void TransportCatalogue::AddStop(std::string name, Coordinates pos) {
		// ������ ����� ��������� � ��������� � � ���������
		stops_.push_back({ std::move(name), pos });

		// ��������� ��������� �� ��������� � ������ �� �����
		auto added_ptr = &stops_.back();
		stop_by_name_[added_ptr->name_] = added_ptr;
	}

	void TransportCatalogue::AddRoute(std::string name, StopsList stops) {
		// ������ ����� ������� � ��������� ��� � ���������
		routes_.push_back({ std::move(name), std::move(stops) });

		// ��������� ��������� �� ������� � ������ �� �����
		auto added_ptr = &routes_.back();
		route_by_name_[added_ptr->name_] = added_ptr;

		// ��������� ������ ��������� � ���������
		for (auto stop : added_ptr->stops_) {
			stop_to_routes_[stop].insert(added_ptr);
		}
	}

	void TransportCatalogue::AddRoute(std::string name, StopsNames stops_names) {
		// ������ ������ ���������� �� ���������
		StopsList stops;
		for (auto stop_name : stops_names) {
			StopPtr stop = FindStop(stop_name);
			if (stop != nullptr) {
				stops.push_back(stop);
			}
		}
		AddRoute(name, stops);
	}

	StopPtr TransportCatalogue::FindStop(std::string_view stop_name) const {
		auto iter = stop_by_name_.find(stop_name);
		return (iter == stop_by_name_.end()) ? nullptr : iter->second;
	}

	RoutePtr TransportCatalogue::FindRoute(std::string_view route_name) const {
		auto iter = route_by_name_.find(route_name);
		return (iter == route_by_name_.end()) ? nullptr : iter->second;
	}

	RouteStatistics TransportCatalogue::GetStat(RoutePtr route) const {
		RouteStatistics stat{};

		// ������� ������ ���������� ���������
		stat.total_stops = route->stops_.size();

		// ������� ���������� ���������
		std::unordered_set<std::string_view> unique_stops;
		for (auto stop : route->stops_) {
			unique_stops.insert(stop->name_);
		}
		stat.unique_stops = unique_stops.size();

		// ������� ����� ��������
		std::optional<Coordinates> prev_pos;
		for (auto stop : route->stops_) {
			if (prev_pos) {
				stat.route_length += ComputeDistance(*prev_pos, stop->coordinates_);
			}
			prev_pos = stop->coordinates_;
		}
		return stat;
	}

	const RouteSet& TransportCatalogue::GetRoutesByStop(StopPtr stop) const {
		static const RouteSet dummy;
		auto iter = stop_to_routes_.find(stop);
		return iter == stop_to_routes_.end() ? dummy : iter->second;
	}

	void TransportCatalogue::SetDistance(StopPtr from, StopPtr to, int distance) {
		distances_[{from, to}] = distance;
	}

	int TransportCatalogue::GetDistance(StopPtr from, StopPtr to) const {
		int distance = -1;	// ��� ������������� ���� ��������� ������������� �������� -1
		
		auto iter = distances_.find({ from, to });
		if (iter != distances_.end()) {
			distance = iter->second;
		}

		return distance;
	}

}	// namespace trans_cat