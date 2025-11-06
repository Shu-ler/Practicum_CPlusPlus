#include "request_handler.h"
#include <algorithm>

namespace request_handler {

    using namespace std::string_literals;

    RequestHandler::RequestHandler(const trans_cat::TransportCatalogue& catalogue)
        : catalogue_(catalogue) {
    }

    std::optional<RouteStat> RequestHandler::GetBusStat(const std::string& bus_name) const {
        const auto* bus = catalogue_.FindRoute(bus_name);
        if (!bus) {
            return std::nullopt;
        }

        RouteStat result;
        result.name = bus_name;
        result.stop_count = bus->stops.size();
        result.unique_stop_count = catalogue_.GetUniqueStopsCount(*bus);

        auto [geo_length, route_length] = catalogue_.GetRouteLength(*bus);
        result.route_length = route_length;
        result.curvature = (geo_length > 1e-6) ? route_length / geo_length : 1.0;

        return result;
    }

    std::optional<StopStat> RequestHandler::GetStopStat(const std::string& stop_name) const {
        const auto* stop = catalogue_.FindStop(stop_name);
        if (!stop) {
            return std::nullopt;
        }

        StopStat result;
        result.name = stop_name;
        const auto& buses = catalogue_.GetBusesByStop(stop_name);
        result.bus_names.assign(buses.begin(), buses.end());
        std::sort(result.bus_names.begin(), result.bus_names.end());

        return result;
    }

} // namespace request_handler