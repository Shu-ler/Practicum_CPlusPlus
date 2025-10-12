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
	}

	void TransportCatalogue::AddRoute(std::string name, std::vector<std::string_view> stops_names) {
		// ������ ������ ���������� �� ���������
		std::vector<StopPtr> stops;
		for (auto stop_name : stops_names) {
			StopPtr stop = FindStop(stop_name);
			if (stop == nullptr) {
				// ���� ��������� �� �������, ����� ������� ��������� �� ������ ��� ���������������
				continue;
			}
			stops.push_back(stop);
		}

		// ������ ����� ������� � ��������� ��� � ���������
		routes_.push_back({ std::move(name), std::move(stops) });

		// ��������� ��������� �� ������� � ������ �� �����
		auto added_ptr = &routes_.back();
		route_by_name_[added_ptr->name_] = added_ptr;
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

}	// namespace trans_cat