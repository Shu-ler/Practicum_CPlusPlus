#pragma once

#include <iosfwd>
#include <string_view>

#include "transport_catalogue.h"

namespace stat_r {

	void ProcessBusRequest(const trans_cat::TransportCatalogue& transport_catalogue,
		const std::string_view& obj_name, std::ostream& output);

	void ProcessStopRequest(const trans_cat::TransportCatalogue& transport_catalogue,
		const std::string_view& obj_name, std::ostream& output);

	void ParseAndPrintStat(const trans_cat::TransportCatalogue& transport_catalogue,
		std::string_view request, std::ostream& output);

} // namespace stat_r