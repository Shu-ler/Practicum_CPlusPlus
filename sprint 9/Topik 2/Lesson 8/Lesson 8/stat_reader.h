#pragma once

#include <iosfwd>
#include <string_view>

#include "transport_catalogue.h"

using RoutePtr = const route::Route*;

void ParseAndPrintStat(const trans_catalogue::TransportCatalogue& tansport_catalogue, 
    std::string_view request,
    std::ostream& output);
