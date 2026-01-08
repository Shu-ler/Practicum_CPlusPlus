// transport_router.cpp
#include "transport_router.h"
#include <cmath>

namespace tr = transport_router;

using namespace std::string_literals;

tr::TransportRouter::TransportRouter(const trans_cat::TransportCatalogue& catalogue)
    : catalogue_(catalogue)
    , graph_(0)
{
}

void tr::TransportRouter::SetRoutingSettings(RoutingSettings settings) {
    settings_ = settings;

    const auto& stops = catalogue_.GetAllStops();
    graph_ = graph::DirectedWeightedGraph<double>(2 * stops.size());

    stop_to_wait_vertex_.clear();
    stop_to_bus_vertex_.clear();
    wait_edge_ids_.clear();
    edge_data_.clear();

    size_t vertex_id = 0;
    for (const auto& [name, stop_ptr] : stops) {
        stop_to_wait_vertex_[name] = vertex_id++;
        stop_to_bus_vertex_[name] = vertex_id++;
    }

    edge_data_.clear(); // будет расти при добавлении рёбер
    BuildGraph();
}

void tr::TransportRouter::BuildGraph() {
    AddWaitEdges();
    for (const auto& route : catalogue_.GetRoutesInInsertionOrder()) {
        AddBusEdges(*route);
    }

    router_.emplace(graph_);
}

void tr::TransportRouter::AddWaitEdges() {
    const double wait_time = settings_.bus_wait_time;

    for (const auto& [name, wait_vertex] : stop_to_wait_vertex_) {
        auto bus_vertex = stop_to_bus_vertex_.at(name);
        auto edge_id = graph_.AddEdge(graph::Edge<double>{
            .from = wait_vertex,
                .to = bus_vertex,
                .weight = wait_time
        });
        wait_edge_ids_.insert(edge_id);
    }
}

void tr::TransportRouter::AddBusEdges(const trans_cat::Route& route) {
    const auto& stops = route.stops;
    const double velocity_m_min = settings_.bus_velocity * 1000.0 / 60.0; // м/мин

    // Прямой путь: от i до j
    for (size_t i = 0; i < stops.size(); ++i) {
        double accumulated_distance = 0.0;

        for (size_t j = i + 1; j < stops.size(); ++j) {
            accumulated_distance += catalogue_.GetDistance(stops[j - 1], stops[j]);
            double time = accumulated_distance / velocity_m_min;

            auto from = stop_to_bus_vertex_.at(stops[i]->name);
            auto to = stop_to_wait_vertex_.at(stops[j]->name);

            auto edge_id = graph_.AddEdge(graph::Edge<double>{
                .from = from,
                    .to = to,
                    .weight = time
            });

            edge_data_.push_back(BusEdgeData{
                .bus_name = route.name,
                .span_count = j - i
                });
        }
    }

    // Обратный путь — только для некольцевых
    if (!route.is_roundtrip) {
        for (size_t i = 0; i < stops.size(); ++i) {
            double accumulated_distance = 0.0;

            // От i до 0 (обратно)
            for (size_t j = i; j > 0; --j) {
                accumulated_distance += catalogue_.GetDistance(stops[j], stops[j - 1]);
                double time = accumulated_distance / velocity_m_min;

                auto from = stop_to_bus_vertex_.at(stops[i]->name);
                auto to = stop_to_wait_vertex_.at(stops[j - 1]->name);

                auto edge_id = graph_.AddEdge(graph::Edge<double>{
                    .from = from,
                        .to = to,
                        .weight = time
                });

                edge_data_.push_back(BusEdgeData{
                    .bus_name = route.name,
                    .span_count = i - j + 1
                    });
            }
        }
    }
}

std::optional<tr::RouteInfo> tr::TransportRouter::BuildRoute(std::string_view from, std::string_view to) const {
    const auto* from_stop = catalogue_.FindStop(from);
    const auto* to_stop = catalogue_.FindStop(to);

    if (!from_stop || !to_stop) {
        return std::nullopt;
    }

    auto from_vertex_it = stop_to_wait_vertex_.find(from);
    auto to_vertex_it = stop_to_wait_vertex_.find(to);

    if (from_vertex_it == stop_to_wait_vertex_.end() || to_vertex_it == stop_to_wait_vertex_.end()) {
        return std::nullopt;
    }

    auto route = router_->BuildRoute(from_vertex_it->second, to_vertex_it->second);
    if (!route) {
        return std::nullopt;
    }

    RouteInfo result;
    result.total_time = route->weight;

    for (graph::EdgeId edge_id : route->edges) {
        if (wait_edge_ids_.count(edge_id)) {
            // Wait-ребро
            auto stop_vertex = graph_.GetEdge(edge_id).from;
            std::string_view stop_name;
            for (const auto& [name, vertex] : stop_to_wait_vertex_) {
                if (vertex == stop_vertex) {
                    stop_name = name;
                    break;
                }
            }

            result.segments.push_back(RouteSegment{
                .type = RouteSegment::Type::Wait,
                .stop_name = std::string(stop_name),
                .time = static_cast<double>(settings_.bus_wait_time)
                });
        }
        else {
            // Bus-ребро
            const auto& data = edge_data_[edge_id].value();
            const auto& edge = graph_.GetEdge(edge_id);

            result.segments.push_back(RouteSegment{
                .type = RouteSegment::Type::Bus,
                .bus_name = data.bus_name,
                .span_count = data.span_count,
                .time = edge.weight
                });
        }
    }

    return result;
}
