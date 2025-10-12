#pragma once

#include "geo.h"
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>

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

	/**
	 * ��������� RouteStatistics - �������� ���������� �������� ������������� ����������.
	 */
	struct RouteStatistics {
		size_t total_stops = 0;      // ����� ���������� ��������� � ��������
		size_t unique_stops = 0;     // ���������� ���������� ���������
		double route_length = 0.0;   // ����� �������� � ������
	};

	class TransportCatalogue {
	public:
		// ����� ���������� ��������� � ����������
		void AddStop(std::string name, Coordinates pos);

		// ����� ���������� �������� � ����������
		void AddRoute(std::string name, StopsList stops);

		// ����� ���������� �������� � ����������
		void AddRoute(std::string name, std::vector<std::string_view> stops_names);

		// ����� ������ ��������� �� �����
		StopPtr FindStop(std::string_view stop_name) const;

		// ����� ������ �������� �� �����
		RoutePtr FindRoute(std::string_view route_name) const;

		// ����� ��� ��������� ���������� �� �������� (���������� ���������, 
		// ���������� ���������, ����� ��������).
		RouteStatistics GetStat(RoutePtr route) const;

	private:
		// ��������� � '������' ���������
		StopsStorage stops_{};
		StopsIndex stop_by_name_{};

		// ��������� � '������' ���������
		RouteStorage routes_{};
		RouteIndex route_by_name_{};
	};

}	// namespace trans_cat