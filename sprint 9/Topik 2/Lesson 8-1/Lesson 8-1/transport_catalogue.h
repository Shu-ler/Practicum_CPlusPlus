#pragma once

#include "geo.h"
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <unordered_set>

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
	using StopsNames = std::vector<std::string_view>;
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
	using RouteSet = std::unordered_set<RoutePtr>;
	using StopRouteIndex = std::unordered_map<StopPtr, RouteSet>;

	/**
	 * Структура RouteStatistics - хранение статистики маршрута общественного транспорта.
	 */
	struct RouteStatistics {
		size_t total_stops = 0;      // Общее количество остановок в маршруте
		size_t unique_stops = 0;     // Количество уникальных остановок
		double route_length = 0.0;   // Длина маршрута в метрах
	};

	/**
	 * Хэшер для пар остановок.
	*/
	struct StopsPairHasher {
		template <typename First, typename Second>
		size_t operator()(const std::pair<First, Second>& obj) const {
			return std::hash<First>()(obj.first) + 37 * std::hash<Second>()(obj.second);
		}
	};

	/**
	 * Справочник транспортного каталога, содержащий информацию о остановках и маршрутах общественного транспорта.
	 * Этот класс отвечает за хранение данных, таких как названия остановок, их географические координаты, а также
	 * маршруты и связанные с ними остановки.
	*/
	class TransportCatalogue {
	public:
		// Метод добавления остановки в справочник
		void AddStop(std::string name, Coordinates pos);

		// Метод добавления маршрута в справочник
		void AddRoute(std::string name, StopsNames stops_names);

		// Метод поиска остановки по имени
		StopPtr FindStop(std::string_view stop_name) const;

		// Метод поиска маршрута по имени
		RoutePtr FindRoute(std::string_view route_name) const;

		// Метод для получения статистики по маршруту (количество остановок, 
		// уникальные остановки, длина маршрута).
		RouteStatistics GetStat(RoutePtr route) const;

		// Возвращает множество маршрутов, проходящих через заданную остановку.
		// Если остановка не найдена или через неё не проходит ни один маршрут,
		// возвращается пустое множеств
		const RouteSet& GetRoutesByStop(StopPtr stop) const;

		// Метод установки расстояния между двумя остановками
		void SetDistance(StopPtr from, StopPtr to, int distance);

		// Метод получения расстояния между двумя остановками
		int GetDistance(StopPtr from, StopPtr to) const;

	private:
		// Метод добавления маршрута в справочник
		void AddRoute(std::string name, StopsList stops);

	private:
		// Контейнер и 'индекс' остановок
		StopsStorage stops_{};
		StopsIndex stop_by_name_{};

		// Контейнер и 'индекс' маршрутов
		RouteStorage routes_{};
		RouteIndex route_by_name_{};

		// 'Индекс' остановок и маршрутов
		StopRouteIndex stop_to_routes_;

		// Контейнер хранения расстояний между остановками
		std::unordered_map<std::pair<StopPtr, StopPtr>, int, StopsPairHasher> distances_;
	};

}	// namespace trans_cat