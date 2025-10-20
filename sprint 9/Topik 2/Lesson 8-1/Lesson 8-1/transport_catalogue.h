#pragma once

#include "geo.h"
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <unordered_set>

namespace trans_cat {

	/**
	 * ��������� Stop - �������� ������ ��������� ������������� ����������.
	 */
	struct  Stop {
		std::string name_{};					// �������� ���������.
		Coordinates coordinates_{ 0.0, 0.0 };	// �������������� ���������� ���������.
	};

	using StopPtr = const Stop*;
	using StopsList = std::vector<StopPtr>;
	using StopsNames = std::vector<std::string_view>;
	using StopsStorage = std::deque<Stop>;
	using StopsIndex = std::unordered_map<std::string_view, Stop*>;

	/**
	 * ��������� Route - �������� ������ �������� ������������� ����������.
	 */
	struct Route {
		std::string name_;	// �������� ��������
		StopsList stops_;	// ������ ��������� (������ ���������� �� ���������)
	};

	using RoutePtr = const Route*;
	using RouteStorage = std::deque<Route>;
	using RouteIndex = std::unordered_map<std::string_view, Route*>;
	using RouteSet = std::unordered_set<RoutePtr>;
	using StopRouteIndex = std::unordered_map<StopPtr, RouteSet>;

	/**
	 * ��������� RouteStatistics - �������� ���������� �������� ������������� ����������.
	 */
	struct RouteStatistics {
		size_t total_stops = 0;      // ����� ���������� ��������� � ��������
		size_t unique_stops = 0;     // ���������� ���������� ���������
		double route_length = 0.0;   // ����� �������� � ������
	};

	/**
	 * ����� ��� ��� ���������.
	*/
	struct StopsPairHasher {
		template <typename First, typename Second>
		size_t operator()(const std::pair<First, Second>& obj) const {
			return std::hash<First>()(obj.first) + 37 * std::hash<Second>()(obj.second);
		}
	};

	/**
	 * ���������� ������������� ��������, ���������� ���������� � ���������� � ��������� ������������� ����������.
	 * ���� ����� �������� �� �������� ������, ����� ��� �������� ���������, �� �������������� ����������, � �����
	 * �������� � ��������� � ���� ���������.
	*/
	class TransportCatalogue {
	public:
		// ����� ���������� ��������� � ����������
		void AddStop(std::string name, Coordinates pos);

		// ����� ���������� �������� � ����������
		void AddRoute(std::string name, StopsNames stops_names);

		// ����� ������ ��������� �� �����
		StopPtr FindStop(std::string_view stop_name) const;

		// ����� ������ �������� �� �����
		RoutePtr FindRoute(std::string_view route_name) const;

		// ����� ��� ��������� ���������� �� �������� (���������� ���������, 
		// ���������� ���������, ����� ��������).
		RouteStatistics GetStat(RoutePtr route) const;

		// ���������� ��������� ���������, ���������� ����� �������� ���������.
		// ���� ��������� �� ������� ��� ����� �� �� �������� �� ���� �������,
		// ������������ ������ ��������
		const RouteSet& GetRoutesByStop(StopPtr stop) const;

		// ����� ��������� ���������� ����� ����� �����������
		void SetDistance(StopPtr from, StopPtr to, int distance);

		// ����� ��������� ���������� ����� ����� �����������
		int GetDistance(StopPtr from, StopPtr to) const;

	private:
		// ����� ���������� �������� � ����������
		void AddRoute(std::string name, StopsList stops);

	private:
		// ��������� � '������' ���������
		StopsStorage stops_{};
		StopsIndex stop_by_name_{};

		// ��������� � '������' ���������
		RouteStorage routes_{};
		RouteIndex route_by_name_{};

		// '������' ��������� � ���������
		StopRouteIndex stop_to_routes_;

		// ��������� �������� ���������� ����� �����������
		std::unordered_map<std::pair<StopPtr, StopPtr>, int, StopsPairHasher> distances_;
	};

}	// namespace trans_cat