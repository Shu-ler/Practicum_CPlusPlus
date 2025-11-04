#include "transport_catalogue.h"
#include <unordered_set>
#include <optional>
#include <ranges>
#include <string_view>

namespace trans_cat {

	StopPtr TransportCatalogue::AddStop(std::string name, Coordinates pos) {
		Stop* added_stop = nullptr;

		// Проверка наличия остановки в каталоге
		auto it = stop_by_name_.find(name);

		// Остановка не найдена - создание новой остановки
		if (it == stop_by_name_.end()) {
			added_stop = CreateStop(name, pos);
		}

		// Остановка найдена - изменение координат 
		else {
			added_stop = it->second;
			SetCoordinates(added_stop, pos);
		}

		return added_stop;
	}

	void TransportCatalogue::AddRoute(std::string name, StopsNames stops_names) {
		// Создаём вектор указателей на остановки
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

		// Подсчёт общего количества остановок
		stat.total_stops = route->stops_.size();

		// Подсчёт уникальных остановок
		std::unordered_set<std::string_view> unique_stops;
		for (auto stop : route->stops_) {
			unique_stops.insert(stop->name_);
		}
		stat.unique_stops = unique_stops.size();

		// Подсчёт длины маршрута в географических координатах и фактических расстояниях
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

		// Вычисление извилистости маршрута
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
		int distance = 0;	// Для отсутствующей пары остановок устанавливаем значение 0

		// Пытаемся найти расстояние от from до to
		auto iter = distances_.find({ from, to });
		if (iter != distances_.end()) {
			distance = iter->second;
		}
		else {
			// Если не нашли - пытаемся найти расстояние от to до from
			auto reverse_iter = distances_.find({ to, from });
			if (reverse_iter != distances_.end()) {
				distance = reverse_iter->second;
			}
		}

		// Возвращаем найденное расстояние.
		// Если не найдено - возвращаем расстояние по географическим координатам
		return (distance == 0) ? ComputeDistance(from->coordinates_, to->coordinates_) : distance;
	}

	void TransportCatalogue::SetDistance(StopPtr from, StopPtr to, int distance) {
		distances_[{from, to}] = distance;
	}

	/*
	 * Создаёт новую остановку и добавляет её в транспортный справочник.
	 *
	 * @param name Имя остановки
	 * @param stopdata Данные об остановке, включая координаты и информацию о соседних остановках
	 * @return Указатель на созданную остановку
	 */
	Stop* TransportCatalogue::CreateStop(std::string& name, Coordinates pos) {

		// Создаём новую остановку и добавляем её в контейнер
		stops_.push_back({ std::move(name), pos });

		// Добавляем указатель на остановку в индекс по имени
		auto added_stop = &stops_.back();
		stop_by_name_[added_stop->name_] = added_stop;

		return added_stop;
	}

	void TransportCatalogue::SetCoordinates(Stop* stop, Coordinates pos) {
		stop->coordinates_.lat = pos.lat;
		stop->coordinates_.lng = pos.lng;
	}

	void TransportCatalogue::AddRoute(std::string name, StopsList stops) {
		// Создаём новый маршрут и добавляем его в контейнер
		routes_.push_back({ std::move(name), std::move(stops) });

		// Добавляем указатель на маршрут в индекс по имени
		auto added_ptr = &routes_.back();
		route_by_name_[added_ptr->name_] = added_ptr;

		// Заполняем индекс остановок и маршрутов
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
