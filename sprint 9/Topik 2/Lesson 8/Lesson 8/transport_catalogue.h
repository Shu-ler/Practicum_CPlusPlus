#pragma once

#include "geo.h"
#include <string>
#include <iostream>
#include <vector>
#include <deque>
#include <unordered_map>

namespace stop {

	/**
	 * Класс представляет собой модель остановки общественного транспорта.
	 * Содержит информацию о названии остановки и её географических координатах.
	 */
	class Stop {
	public:

		// Конструктор по умолчанию
		Stop() = default;

		// Инициализирует новую остановку с заданными именем и координатами.
		// @param name название остановки
		// @param lat широта
		// @param lon долгота
		Stop(const std::string& name, const double lat, const double lon) :
			name_(name),
			coordinates_{ lat, lon } {
		};

		// Инициализирует новую остановку с заданными именем и координатами.
		// @param name название остановки
		// @param pos географические координаты
		Stop(const std::string& name, const Coordinates pos) :
			name_(name),
			coordinates_{ pos } {
		};

		// Возвращает название остановки.
		// @return название остановки
		std::string GetName() const {
			return name_;
		}

		// Возвращает координаты остановки.
		// @return координаты остановки
		Coordinates GetCoordinates() const {
			return coordinates_;
		}

		// Метод установки координат
		// @param lat широта
		// @param lon долгота
		void SetCoordinates(const double lat, const double lon) {
			coordinates_ = { lat, lon };
		}

		// Переопределение оператора равенства для остановок остановок
		// @param other вторая остановка
		bool operator==(const Stop& other) const {
			return name_ == other.name_ && coordinates_ == other.coordinates_;
		}

		// Вывод информации об остановке
		// @param os поток вывода
		// @param stop остановка
		friend std::ostream& operator<<(std::ostream& os, const Stop& stop) {
			using namespace std::literals;
			os << "Stop: "s << stop.name_
				<< ", Coordinates: ("s << stop.coordinates_.lat
				<< ", "s << stop.coordinates_.lng
				<< ")"s;
			return os;
		}

	private:
		std::string name_{};					// Название остановки.
		Coordinates coordinates_{ 0.0, 0.0 };	// Географические координаты остановки.
	};

} // namespace stop

namespace bus {

	/**
	 * Класс Bus моделирует автобусный маршрут и содержит следующие компоненты : 
	 *  - name_ : строка, хранящая название маршрута.
	 *  - stops_ : вектор указателей на остановки(StopPtr), 
	 *		представляющий последовательность остановок в маршруте.
	 */
	class Bus {
	public:
		using StopPtr = const stop::Stop*;
		using RouteStops = std::vector<StopPtr>;

		// Конструктор по умолчанию
		Bus() = default;

		// Параметрический конструктор
		Bus(const std::string& name, const RouteStops& stops);

		// Возвращает название остановки.
		// @return название остановки
		//std::string GetName() const;

		// Getter для stops_
		const RouteStops& GetStops() const;

		// Setter для stops_
		void SetStops(const RouteStops& stops);

		// Метод для добавления остановки
		void AddStop(StopPtr stop);


		// Возвращает true, если маршрут кольцевой
		static bool IsRingRoute(const std::string& route);

		// Возвращает true, если маршрут обычный
		static bool IsOrdinaryRoute(const std::string& route);

	private:
		std::string name_;	// название маршрута
		RouteStops stops_;	// вектор указателей на остановки
	};

} // namespace bus

namespace trans_catalogue {

	class TransportCatalogue {
	public:
		using StopPtr = const stop::Stop*;
		using BusPtr = const bus::Bus*;

		struct RouteStatistics {
			size_t total_stops = 0;      // Общее количество остановок в маршруте
			size_t unique_stops = 0;     // Количество уникальных остановок
			double route_length = 0.0;   // Длина маршрута в метрах
		};

		// Метод добавления остановки в справочник
		void AddStop(std::string name, Coordinates pos);

		// Метод добавления маршрута в справочник
		void AddBus(std::string name, std::vector<StopPtr> stops);

		// Метод поиска остановки по имени
		StopPtr FindStop(std::string_view bus_name) const;

		// Метод поиска маршрута по имени
		BusPtr FindBus(std::string_view bus_name) const;

		// Метод для получения статистики по маршруту (количество остановок, 
		// уникальные остановки, длина маршрута).
		RouteStatistics GetStat() const;

	private:
		// Контейнер и 'индекс' остановок
		std::deque<stop::Stop> stops_{};
		std::unordered_map<std::string_view, StopPtr> stop_by_name_{};

		// Контейнер и 'индекс' маршрутов
		std::deque<bus::Bus> buses_{};
		std::unordered_map<std::string_view, BusPtr> bus_by_name_{};
	};

} // namespace trans_catalogue

// Используя 'using', определяем псевдонимы для удобства
using StopPtr = const stop::Stop*;
using BusPtr = const bus::Bus*;
using RouteStops = std::vector<StopPtr>;

