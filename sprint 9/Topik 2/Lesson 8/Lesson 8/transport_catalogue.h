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

namespace route {

	/**
	 * Класс Route моделирует автобусный маршрут и содержит следующие компоненты : 
	 *  - name_ : строка, хранящая название маршрута.
	 *  - stops_ : вектор указателей на остановки(StopPtr), 
	 *		представляющий последовательность остановок в маршруте.
	 */
	class Route {
	public:
		using StopPtr = const stop::Stop*;
		using RouteStops = std::vector<StopPtr>;

		// Конструктор по умолчанию
		Route() = default;

		// Параметрический конструктор
		Route(const std::string& name, const RouteStops& stops);

		// Возвращает название маршрута.
		// @return название маршрута
		std::string GetName() const {
			return name_;
		};

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

} // namespace route

namespace trans_catalogue {

	class TransportCatalogue {
	public:
		using StopPtr = const stop::Stop*;
		using StopsIndex = std::unordered_map<std::string_view, StopPtr>;
		using RoutePtr = const route::Route*;

		struct RouteStatistics {
			size_t total_stops = 0;      // Общее количество остановок в маршруте
			size_t unique_stops = 0;     // Количество уникальных остановок
			double route_length = 0.0;   // Длина маршрута в метрах
		};

		// Метод добавления остановки в справочник
		void AddStop(std::string_view name, Coordinates pos);

		// Метод добавления маршрута в справочник
		void AddRoute(std::string name, std::vector<StopPtr> stops);

		// Метод поиска остановки по имени
		StopPtr FindStop(std::string_view stop_name) const;

		// Метод поиска маршрута по имени
		RoutePtr FindRoute(std::string_view route_name) const;

		// Метод для получения статистики по маршруту (количество остановок, 
		// уникальные остановки, длина маршрута).
		RouteStatistics GetStat(RoutePtr route) const;

		StopsIndex GetStopsIndex();
		StopsIndex GetStopsIndex() const;

	private:
		// Контейнер и 'индекс' остановок
		std::deque<stop::Stop> stops_{};
		StopsIndex stop_by_name_{};

		// Контейнер и 'индекс' маршрутов
		std::deque<route::Route> buses_{};
		std::unordered_map<std::string_view, RoutePtr> bus_by_name_{};
	};

} // namespace trans_catalogue

// Используя 'using', определяем псевдонимы для удобства
using StopPtr = const stop::Stop*;
using RoutePtr = const route::Route*;
using RouteStops = std::vector<StopPtr>;

