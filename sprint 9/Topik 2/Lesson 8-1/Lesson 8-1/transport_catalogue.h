#pragma once

#include "geo.h"
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>

namespace trans_cat {

	/**
	 * Структура Stop - хранение данных остановки общественного транспорта.
	 */
	struct  Stop {
		std::string name_{};					// Название остановки.
		Coordinates coordinates_{ 0.0, 0.0 };	// Географические координаты остановки.
	};

	using StopPtr = const Stop*;
	using StopsList = std::vector<StopPtr>;
	using StopsStorage = std::deque<Stop>;
	using StopsIndex = std::unordered_map<std::string_view, Stop*>;

	/**
	 * Структура Route - хранение данных маршрута общественного транспорта.
	 */
	struct Route {
		std::string name_;	// Название маршрута
		StopsList stops_;	// Список остановок (вектор указателей на остановки)
	};

	using RoutePtr = const Route*;
	using RouteStorage = std::deque<Route>;
	using RouteIndex = std::unordered_map<std::string_view, Route*>;

	/**
	 * Структура RouteStatistics - хранение статистики маршрута общественного транспорта.
	 */
	struct RouteStatistics {
		size_t total_stops = 0;      // Общее количество остановок в маршруте
		size_t unique_stops = 0;     // Количество уникальных остановок
		double route_length = 0.0;   // Длина маршрута в метрах
	};

	class TransportCatalogue {
	public:
		// Метод добавления остановки в справочник
		void AddStop(std::string name, Coordinates pos);

		// Метод добавления маршрута в справочник
		void AddRoute(std::string name, StopsList stops);

		// Метод добавления маршрута в справочник
		void AddRoute(std::string name, std::vector<std::string_view> stops_names);

		// Метод поиска остановки по имени
		StopPtr FindStop(std::string_view stop_name) const;

		// Метод поиска маршрута по имени
		RoutePtr FindRoute(std::string_view route_name) const;

		// Метод для получения статистики по маршруту (количество остановок, 
		// уникальные остановки, длина маршрута).
		RouteStatistics GetStat(RoutePtr route) const;

	private:
		// Контейнер и 'индекс' остановок
		StopsStorage stops_{};
		StopsIndex stop_by_name_{};

		// Контейнер и 'индекс' маршрутов
		RouteStorage routes_{};
		RouteIndex route_by_name_{};
	};

}	// namespace trans_cat