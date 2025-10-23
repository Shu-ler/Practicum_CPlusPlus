#include "transport_catalogue.h"
#include <unordered_set>
#include <optional>
#include <ranges>
#include <string_view>

namespace trans_cat {

	StopPtr TransportCatalogue::AddStop(std::string name, StopData stopdata) {
		Stop* added_stop = nullptr;

		// �������� ������� ��������� � ��������
		auto it = stop_by_name_.find(name);

		// ��������� �� ������� - �������� ����� ���������
		if (it == stop_by_name_.end()) {
			added_stop = CreateStop(name, stopdata);
		} 

		// ��������� ������� - ��������� ��������� 
		else {
			added_stop = it->second;
			SetCoordinates(added_stop, stopdata);
		}

		// ���������� ������ �������� ���������
		SetDistances(added_stop, stopdata);

		return added_stop;
	}

	void TransportCatalogue::AddRoute(std::string name, StopsNames stops_names) {
		// ������ ������ ���������� �� ���������
		StopsList stops;
		for (auto stop_name : stops_names) {
			StopPtr stop = FindStop(stop_name);
			if (stop == nullptr) {
				std::string buffer_name(stop_name);
				stop = CreateStopWithoutData(buffer_name);
			}
			stops.push_back(stop);

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

		// ������� ����� �������� � �������������� ����������� � ����������� �����������
		std::optional<Coordinates> prev_pos;
		std::optional<StopPtr> prev_stop;
		for (auto stop : route->stops_) {
			if (prev_pos) {
				stat.route_length_direct += ComputeDistance(*prev_pos, stop->coordinates_);
				if (prev_stop) {
					int road_distance = GetDistance(*prev_stop, stop);
					stat.route_length += road_distance;
				}
			}
			prev_pos = stop->coordinates_;
			prev_stop = stop;
		}

		// ������ ������������ ��������
		stat.curvature = (stat.route_length_direct > 0) 
			? stat.route_length / stat.route_length_direct 
			: 0;

		return stat;
	}

	const RouteSet& TransportCatalogue::GetRoutesByStop(StopPtr stop) const {
		static const RouteSet dummy;
		auto iter = stop_to_routes_.find(stop);
		return iter == stop_to_routes_.end() ? dummy : iter->second;
	}

	int TransportCatalogue::GetDistance(StopPtr from, StopPtr to) const {
		int distance = 0;	// ��� ������������� ���� ��������� ������������� �������� 0

		// �������� ����� ���������� �� from �� to
		auto iter = distances_.find({ from, to });
		if (iter != distances_.end()) {
			distance = iter->second;
		}
		else {
			// ���� �� �����, �������� ����� ���������� �� to �� from
			auto reverse_iter = distances_.find({ to, from });
			if (reverse_iter != distances_.end()) {
				distance = reverse_iter->second;
			}
		}

		return distance;
	}

	/*
	 * ������ ����� ��������� � ��������� � � ������������ ����������.
	 *
	 * @param name ��� ���������
	 * @param stopdata ������ �� ���������, ������� ���������� � ���������� � �������� ����������
	 * @return ��������� �� ��������� ���������
	 */
	Stop* TransportCatalogue::CreateStop(std::string& name, StopData& stopdata) {
		
		// ������ ����� ��������� � ��������� � � ���������
		stops_.push_back({ std::move(name), stopdata.coordinates });

		// ��������� ��������� �� ��������� � ������ �� �����
		auto added_stop = &stops_.back();
		stop_by_name_[added_stop->name_] = added_stop;

		return added_stop;
	}

	StopPtr TransportCatalogue::CreateStopWithoutData(std::string& name) {
		StopData blank_data{};
		return AddStop(name, blank_data);
	}

	void TransportCatalogue::SetCoordinates(Stop* stop, StopData& stopdata) {
		stop->coordinates_.lat = stopdata.coordinates.lat;
		stop->coordinates_.lng = stopdata.coordinates.lng;
	}


	void TransportCatalogue::SetDistance(StopPtr from, StopPtr to, int distance) {
		distances_[{from, to}] = distance;
	}

	void TransportCatalogue::SetDistances(Stop* added_stop, StopData& stopdata) {
		for (const auto& stop : stopdata.nearby_stops) {
			auto target_stop = FindStop(stop.first);
			if (target_stop == nullptr) {
				std::string stop_name(stop.first);
				target_stop = CreateStopWithoutData(stop_name);
			}
			SetDistance(added_stop, target_stop, stop.second);
		}
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


}	// namespace trans_cat