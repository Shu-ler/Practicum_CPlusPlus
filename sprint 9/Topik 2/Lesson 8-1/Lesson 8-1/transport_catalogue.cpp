#include "transport_catalogue.h"
#include <unordered_set>
#include <optional>
#include <ranges>
#include <string_view>

namespace trans_cat {

	StopPtr TransportCatalogue::AddStop(std::string name, Coordinates pos) {
		Stop* added_stop = nullptr;

		// �������� ������� ��������� � ��������
		auto it = stop_by_name_.find(name);

		// ��������� �� ������� - �������� ����� ���������
		if (it == stop_by_name_.end()) {
			added_stop = CreateStop(name, pos);
		}

		// ��������� ������� - ��������� ��������� 
		else {
			added_stop = it->second;
			SetCoordinates(added_stop, pos);
		}

		return added_stop;
	}

	void TransportCatalogue::AddRoute(std::string name, StopsNames stops_names) {
		// ������ ������ ���������� �� ���������
		StopsList stops;
		for (auto stop_name : stops_names) {
			StopPtr stop = FindStop(stop_name);
			if (stop == nullptr) {
				std::string buffer_name(stop_name);
				stop = AddStop(buffer_name, {0,0});
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

		// ���������� ������������ ��������
		stat.curvature = (stat.route_length_direct > 0)
			? stat.route_length / stat.route_length_direct
			: 0;

		return stat;
	}

	const std::set<std::string_view> TransportCatalogue::GetRoutesNamesByStop(StopPtr stop) const {
		std::set<std::string_view> routes_name;

		auto routes_set = GetRoutesByStop(stop);
		for (const auto& route : routes_set) {
			routes_name.insert(route->name_);
		}

		return routes_name;
	}

	int TransportCatalogue::GetDistance(StopPtr from, StopPtr to) const {
		int distance = 0;	// ��� ������������� ���� ��������� ������������� �������� 0

		// �������� ����� ���������� �� from �� to
		auto iter = distances_.find({ from, to });
		if (iter != distances_.end()) {
			distance = iter->second;
		}
		else {
			// ���� �� ����� - �������� ����� ���������� �� to �� from
			auto reverse_iter = distances_.find({ to, from });
			if (reverse_iter != distances_.end()) {
				distance = reverse_iter->second;
			}
		}

		// ���������� ��������� ����������.
		// ���� �� ������� - ���������� ���������� �� �������������� �����������
		return (distance == 0) ? ComputeDistance(from->coordinates_, to->coordinates_) : distance;
	}

	void TransportCatalogue::SetDistance(StopPtr from, StopPtr to, int distance) {
		distances_[{from, to}] = distance;
	}

	/*
	 * ������ ����� ��������� � ��������� � � ������������ ����������.
	 *
	 * @param name ��� ���������
	 * @param stopdata ������ �� ���������, ������� ���������� � ���������� � �������� ����������
	 * @return ��������� �� ��������� ���������
	 */
	Stop* TransportCatalogue::CreateStop(std::string& name, Coordinates pos) {

		// ������ ����� ��������� � ��������� � � ���������
		stops_.push_back({ std::move(name), pos });

		// ��������� ��������� �� ��������� � ������ �� �����
		auto added_stop = &stops_.back();
		stop_by_name_[added_stop->name_] = added_stop;

		return added_stop;
	}

	void TransportCatalogue::SetCoordinates(Stop* stop, Coordinates pos) {
		stop->coordinates_.lat = pos.lat;
		stop->coordinates_.lng = pos.lng;
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

	const RouteSet& TransportCatalogue::GetRoutesByStop(StopPtr stop) const {
		static const RouteSet dummy;
		auto iter = stop_to_routes_.find(stop);
		return iter == stop_to_routes_.end() ? dummy : iter->second;
	}

}	// namespace trans_cat
