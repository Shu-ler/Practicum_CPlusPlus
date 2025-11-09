#include "transport_catalogue.h"
#include "geo.h"
#include <cmath>

namespace trans_cat {

    // Хэш для пары указателей
    size_t TransportCatalogue::PairHash::operator()(
        const std::pair<const Stop*, const Stop*>& p) const {
        size_t seed = 0;
        hash_combine(seed, p.first);
        hash_combine(seed, p.second);
        return seed;
    }

    const Stop* TransportCatalogue::AddStop(std::string_view name, geo::Coordinates coords) {
        if (auto* stop = FindStop(name)) {
            const_cast<Stop*>(stop)->coordinates = coords;
            return stop;
        }

        stops_.push_back(Stop{ std::string(name), coords });
        const Stop* stop_ptr = &stops_.back();
        stopname_to_stop_[stop_ptr->name] = stop_ptr;
        return stop_ptr;
    }

    void TransportCatalogue::AddRoute(std::string name, const std::vector<std::string>& stops,
        bool is_roundtrip) {
        if (stops.empty()) {
            return;
        }

        // Преобразуем имена в указатели
        std::vector<const Stop*> stop_ptrs;
        stop_ptrs.reserve(stops.size());

        for (const auto& stop_name : stops) {
            if (const Stop* stop = FindStop(stop_name)) {

                // Добавляем существующую остановку
                stop_ptrs.push_back(stop);
            }
            else {
                // Создаём заглушку через единый интерфейс
                const Stop* new_stop = AddStop(stop_name, {});

                // И добавляем её
                stop_ptrs.push_back(new_stop);
            }
        }

        // Если маршрут не кольцевой, делаем двусторонний маршрут
        std::vector<const Stop*> full_route = stop_ptrs;
        if (!is_roundtrip && stop_ptrs.size() > 1) {
            full_route.insert(full_route.end(), std::next(stop_ptrs.rbegin()), stop_ptrs.rend());
        }

        // Создаём маршрут
        routes_.push_back(Route{ std::move(name), std::move(full_route), is_roundtrip });
        const Route* route_ptr = &routes_.back();
        routename_to_route_[route_ptr->name] = route_ptr;

        // Обновляем индекс: остановка → маршрут
        for (const Stop* stop : route_ptr->stops) {
            stop_to_routes_[stop].insert(route_ptr);
        }
    }

    const Stop* TransportCatalogue::FindStop(std::string_view name) const {
        auto it = stopname_to_stop_.find(name);
        return (it != stopname_to_stop_.end()) ? it->second : nullptr;
    }

    const Route* TransportCatalogue::FindRoute(std::string_view name) const {
        auto it = routename_to_route_.find(name);
        return (it != routename_to_route_.end()) ? it->second : nullptr;
    }

    bool TransportCatalogue::StopExists(std::string_view name) const {
        return stopname_to_stop_.contains(name);
    }

    std::optional<TransportCatalogue::RouteStat> TransportCatalogue::GetRouteStat(
        std::string_view route_name) const {

        const Route* route = FindRoute(route_name);
        if (!route || route->stops.empty()) {
            return std::nullopt;
        }

        RouteStat stat;
        stat.stop_count = route->stops.size();

        // Считаем уникальные остановки
        std::set<std::string_view> unique_stops;
        for (const Stop* stop : route->stops) {
            unique_stops.insert(stop->name);
        }
        stat.unique_stop_count = unique_stops.size();

        // Считаем длину маршрута и прямое расстояние
        double route_length_direct = 0.0;
        const Stop* prev_stop = nullptr;
        geo::Coordinates prev_coords;

        for (const Stop* stop : route->stops) {
            if (prev_stop) {
                stat.route_length += GetDistance(prev_stop, stop);
                route_length_direct += ComputeDistance(prev_coords, stop->coordinates);
            }
            prev_stop = stop;
            prev_coords = stop->coordinates;
        }

        stat.curvature = (route_length_direct > 1e-6)
            ? stat.route_length / route_length_direct
            : 1.0;

        return stat;
    }

    std::set<std::string> TransportCatalogue::GetBusesByStop(std::string_view stop_name) const {
        std::set<std::string> result;
        const Stop* stop = FindStop(stop_name);
        if (!stop) {
            return result;
        }

        auto it = stop_to_routes_.find(stop);
        if (it != stop_to_routes_.end()) {
            for (const Route* route : it->second) {
                result.insert(route->name);
            }
        }

        return result;
    }

    void TransportCatalogue::SetDistance(std::string_view from_name, std::string_view to_name,
        int distance) {
        const Stop* from = FindStop(from_name);
        const Stop* to = FindStop(to_name);
        if (from && to) {
            distances_[{from, to}] = distance;
        }
    }

    int TransportCatalogue::GetDistance(const Stop* from, const Stop* to) const {
        if (!from || !to) {
            return 0;
        }

        auto it = distances_.find({ from, to });
        if (it != distances_.end()) {
            return it->second;
        }

        auto r_it = distances_.find({ to, from });
        if (r_it != distances_.end()) {
            return r_it->second;
        }

        // По умолчанию - расстояние по прямой (fallback)
        return static_cast<int>(std::round(ComputeDistance(from->coordinates, to->coordinates)));
    }

} // namespace trans_cat
