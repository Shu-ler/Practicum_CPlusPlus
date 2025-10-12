#pragma once

#include "geo.h"
#include <string>
#include <iostream>
#include <vector>
#include <deque>
#include <unordered_map>

namespace stop {

	/**
	 * ����� ������������ ����� ������ ��������� ������������� ����������.
	 * �������� ���������� � �������� ��������� � � �������������� �����������.
	 */
	class Stop {
	public:

		// ����������� �� ���������
		Stop() = default;

		// �������������� ����� ��������� � ��������� ������ � ������������.
		// @param name �������� ���������
		// @param lat ������
		// @param lon �������
		Stop(const std::string& name, const double lat, const double lon) :
			name_(name),
			coordinates_{ lat, lon } {
		};

		// �������������� ����� ��������� � ��������� ������ � ������������.
		// @param name �������� ���������
		// @param pos �������������� ����������
		Stop(const std::string& name, const Coordinates pos) :
			name_(name),
			coordinates_{ pos } {
		};

		// ���������� �������� ���������.
		// @return �������� ���������
		std::string GetName() const {
			return name_;
		}

		// ���������� ���������� ���������.
		// @return ���������� ���������
		Coordinates GetCoordinates() const {
			return coordinates_;
		}

		// ����� ��������� ���������
		// @param lat ������
		// @param lon �������
		void SetCoordinates(const double lat, const double lon) {
			coordinates_ = { lat, lon };
		}

		// ��������������� ��������� ��������� ��� ��������� ���������
		// @param other ������ ���������
		bool operator==(const Stop& other) const {
			return name_ == other.name_ && coordinates_ == other.coordinates_;
		}

		// ����� ���������� �� ���������
		// @param os ����� ������
		// @param stop ���������
		friend std::ostream& operator<<(std::ostream& os, const Stop& stop) {
			using namespace std::literals;
			os << "Stop: "s << stop.name_
				<< ", Coordinates: ("s << stop.coordinates_.lat
				<< ", "s << stop.coordinates_.lng
				<< ")"s;
			return os;
		}

	private:
		std::string name_{};					// �������� ���������.
		Coordinates coordinates_{ 0.0, 0.0 };	// �������������� ���������� ���������.
	};

} // namespace stop

namespace route {

	/**
	 * ����� Route ���������� ���������� ������� � �������� ��������� ���������� : 
	 *  - name_ : ������, �������� �������� ��������.
	 *  - stops_ : ������ ���������� �� ���������(StopPtr), 
	 *		�������������� ������������������ ��������� � ��������.
	 */
	class Route {
	public:
		using StopPtr = const stop::Stop*;
		using RouteStops = std::vector<StopPtr>;

		// ����������� �� ���������
		Route() = default;

		// ��������������� �����������
		Route(const std::string& name, const RouteStops& stops);

		// ���������� �������� ��������.
		// @return �������� ��������
		std::string GetName() const {
			return name_;
		};

		// Getter ��� stops_
		const RouteStops& GetStops() const;

		// Setter ��� stops_
		void SetStops(const RouteStops& stops);

		// ����� ��� ���������� ���������
		void AddStop(StopPtr stop);


		// ���������� true, ���� ������� ���������
		static bool IsRingRoute(const std::string& route);

		// ���������� true, ���� ������� �������
		static bool IsOrdinaryRoute(const std::string& route);

	private:
		std::string name_;	// �������� ��������
		RouteStops stops_;	// ������ ���������� �� ���������
	};

} // namespace route

namespace trans_catalogue {

	class TransportCatalogue {
	public:
		using StopPtr = const stop::Stop*;
		using StopsIndex = std::unordered_map<std::string_view, StopPtr>;
		using RoutePtr = const route::Route*;

		struct RouteStatistics {
			size_t total_stops = 0;      // ����� ���������� ��������� � ��������
			size_t unique_stops = 0;     // ���������� ���������� ���������
			double route_length = 0.0;   // ����� �������� � ������
		};

		// ����� ���������� ��������� � ����������
		void AddStop(std::string_view name, Coordinates pos);

		// ����� ���������� �������� � ����������
		void AddRoute(std::string name, std::vector<StopPtr> stops);

		// ����� ������ ��������� �� �����
		StopPtr FindStop(std::string_view stop_name) const;

		// ����� ������ �������� �� �����
		RoutePtr FindRoute(std::string_view route_name) const;

		// ����� ��� ��������� ���������� �� �������� (���������� ���������, 
		// ���������� ���������, ����� ��������).
		RouteStatistics GetStat(RoutePtr route) const;

		StopsIndex GetStopsIndex();
		StopsIndex GetStopsIndex() const;

	private:
		// ��������� � '������' ���������
		std::deque<stop::Stop> stops_{};
		StopsIndex stop_by_name_{};

		// ��������� � '������' ���������
		std::deque<route::Route> buses_{};
		std::unordered_map<std::string_view, RoutePtr> bus_by_name_{};
	};

} // namespace trans_catalogue

// ��������� 'using', ���������� ���������� ��� ��������
using StopPtr = const stop::Stop*;
using RoutePtr = const route::Route*;
using RouteStops = std::vector<StopPtr>;

