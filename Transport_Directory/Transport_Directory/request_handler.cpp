#include "request_handler.h"
#include <algorithm>

namespace request_handler {

    using namespace std::string_literals;

    RequestHandler::RequestHandler(const trans_cat::TransportCatalogue& catalogue)
        : catalogue_(catalogue) {
    }

    std::optional<RouteStat> RequestHandler::GetBusStat(const std::string& bus_name) const {
        auto stat = catalogue_.GetRouteStat(bus_name);
        if (!stat) {
            return std::nullopt;
        }

        RouteStat result;
        result.name = bus_name;
        result.stop_count = stat->stop_count;
        result.unique_stop_count = stat->unique_stop_count;
        result.route_length = stat->route_length;
        result.curvature = stat->curvature;

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