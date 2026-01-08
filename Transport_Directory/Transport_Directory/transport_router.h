// transport_router.h
#pragma once

#include "transport_catalogue.h"
#include "graph.h"
#include "router.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace transport_router {

    struct RoutingSettings {
        int bus_wait_time = 0;        // минуты
        double bus_velocity = 0.0;    // км/ч
    };

    struct RouteSegment {
        enum class Type {
            Wait,
            Bus
        };

        Type type;

        // Для Wait
        std::string stop_name;

        // Для Bus
        std::string bus_name;
        size_t span_count = 0;  // количество перегонов
        double time = 0.0;      // время в минутах
    };

    struct RouteInfo {
        double total_time = 0.0;
        std::vector<RouteSegment> segments;
    };

    // ✅ Перемещено вверх: объявление BusEdgeData
    struct BusEdgeData {
        std::string bus_name;
        size_t span_count;
    };

    class TransportRouter {
    public:
        explicit TransportRouter(const trans_cat::TransportCatalogue& catalogue);
        void SetRoutingSettings(RoutingSettings settings);
        std::optional<RouteInfo> BuildRoute(std::string_view from, std::string_view to) const;

    private:
        void BuildGraph();
        void AddWaitEdges();
        void AddBusEdges(const trans_cat::Route& route);

        const trans_cat::TransportCatalogue& catalogue_;
        RoutingSettings settings_;

        graph::DirectedWeightedGraph<double> graph_;

        std::unordered_map<std::string_view, graph::VertexId> stop_to_wait_vertex_;
        std::unordered_map<std::string_view, graph::VertexId> stop_to_bus_vertex_;

        std::unordered_set<graph::EdgeId> wait_edge_ids_;

        // Теперь знает, что такое BusEdgeData
        std::vector<std::optional<BusEdgeData>> edge_data_;

        mutable std::optional<graph::Router<double>> router_;
    };

} // namespace transport_router
