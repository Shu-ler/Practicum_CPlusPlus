#include "transport_catalogue.h"
#include <unordered_set>
#include <optional>
#include <ranges>
#include <string_view>

namespace trans_cat {

	StopPtr TransportCatalogue::AddStop(std::string name, StopData stopdata) {
		Stop* added_stop = nullptr;

		// Проверка наличия остановки в каталоге
		auto it = stop_by_name_.find(name);

		// Остановка не найдена - создание новой остановки
		if (it == stop_by_name_.end()) {
			added_stop = CreateStop(name, stopdata);
		} 

		// Остановка найдена - изменение координат 
		else {
			added_stop = it->second;
			SetCoordinates(added_stop, stopdata);
		}

		// Заполнение данных соседних остановок
		SetDistances(added_stop, stopdata);

		return added_stop;
	}

	void TransportCatalogue::AddRoute(std::string name, StopsNames stops_names) {
		// Создаём вектор указателей на остановки
		StopsList stops;
		for (auto stop_name : stops_names) {
			StopPtr stop = FindStop(stop_name);
			if (stop == nullptr) {
				std::string buffer_name(stop_name);
				stop = CreateStopWithoutData(buffer_name);
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

		// Расчёт извилистости маршрута
		stat.curvature = (stat.route_length_direct > 0) 
			? stat.route_length / stat.route_length_direct 
			: 0;

		return stat;
	}

	const RouteSet& TransportCatalogue::GetRoutesByStop(StopPtr stop) const {
		static const RouteSet dummy;
		auto iter = stop_to_routes_.find(stop);
		return iter == stop_to_routes_.end() ? dummy : iter->second;
	}

	int TransportCatalogue::GetDistance(StopPtr from, StopPtr to) const {
		int distance = 0;	// Для отсутствующей пары остановок устанавливаем значение 0

		// Пытаемся найти расстояние от from до to
		auto iter = distances_.find({ from, to });
		if (iter != distances_.end()) {
			distance = iter->second;
		}
		else {
			// Если не нашли, пытаемся найти расстояние от to до from
			auto reverse_iter = distances_.find({ to, from });
			if (reverse_iter != distances_.end()) {
				distance = reverse_iter->second;
			}
		}

		return distance;
	}

	/*
	 * Создаёт новую остановку и добавляет её в транспортный справочник.
	 *
	 * @param name Имя остановки
	 * @param stopdata Данные об остановке, включая координаты и информацию о соседних остановках
	 * @return Указатель на созданную остановку
	 */
	Stop* TransportCatalogue::CreateStop(std::string& name, StopData& stopdata) {
		
		// Создаём новую остановку и добавляем её в контейнер
		stops_.push_back({ std::move(name), stopdata.coordinates });

		// Добавляем указатель на остановку в индекс по имени
		auto added_stop = &stops_.back();
		stop_by_name_[added_stop->name_] = added_stop;

		return added_stop;
	}

	StopPtr TransportCatalogue::CreateStopWithoutData(std::string& name) {
		StopData blank_data{};
		return AddStop(name, blank_data);
	}

	void TransportCatalogue::SetCoordinates(Stop* stop, StopData& stopdata) {
		stop->coordinates_.lat = stopdata.coordinates.lat;
		stop->coordinates_.lng = stopdata.coordinates.lng;
	}


	void TransportCatalogue::SetDistance(StopPtr from, StopPtr to, int distance) {
		distances_[{from, to}] = distance;
	}

	void TransportCatalogue::SetDistances(Stop* added_stop, StopData& stopdata) {
		for (const auto& stop : stopdata.nearby_stops) {
			auto target_stop = FindStop(stop.first);
			if (target_stop == nullptr) {
				std::string stop_name(stop.first);
				target_stop = CreateStopWithoutData(stop_name);
			}
			SetDistance(added_stop, target_stop, stop.second);
		}
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


}	// namespace trans_cat