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

namespace bus {

	/**
	 * ����� Bus ���������� ���������� ������� � �������� ��������� ���������� : 
	 *  - name_ : ������, �������� �������� ��������.
	 *  - stops_ : ������ ���������� �� ���������(StopPtr), 
	 *		�������������� ������������������ ��������� � ��������.
	 */
	class Bus {
	public:
		using StopPtr = const stop::Stop*;
		using RouteStops = std::vector<StopPtr>;

		// ����������� �� ���������
		Bus() = default;

		// ��������������� �����������
		Bus(const std::string& name, const RouteStops& stops);

		// ���������� �������� ���������.
		// @return �������� ���������
		//std::string GetName() const;

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

} // namespace bus

namespace trans_catalogue {

	class TransportCatalogue {
	public:
		using StopPtr = const stop::Stop*;
		using BusPtr = const bus::Bus*;

		struct RouteStatistics {
			size_t total_stops = 0;      // ����� ���������� ��������� � ��������
			size_t unique_stops = 0;     // ���������� ���������� ���������
			double route_length = 0.0;   // ����� �������� � ������
		};

		// ����� ���������� ��������� � ����������
		void AddStop(std::string name, Coordinates pos);

		// ����� ���������� �������� � ����������
		void AddBus(std::string name, std::vector<StopPtr> stops);

		// ����� ������ ��������� �� �����
		StopPtr FindStop(std::string_view bus_name) const;

		// ����� ������ �������� �� �����
		BusPtr FindBus(std::string_view bus_name) const;

		// ����� ��� ��������� ���������� �� �������� (���������� ���������, 
		// ���������� ���������, ����� ��������).
		RouteStatistics GetStat() const;

	private:
		// ��������� � '������' ���������
		std::deque<stop::Stop> stops_{};
		std::unordered_map<std::string_view, StopPtr> stop_by_name_{};

		// ��������� � '������' ���������
		std::deque<bus::Bus> buses_{};
		std::unordered_map<std::string_view, BusPtr> bus_by_name_{};
	};

} // namespace trans_catalogue

// ��������� 'using', ���������� ���������� ��� ��������
using StopPtr = const stop::Stop*;
using BusPtr = const bus::Bus*;
using RouteStops = std::vector<StopPtr>;

