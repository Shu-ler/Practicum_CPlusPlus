#pragma once

#include <iosfwd>
#include <string_view>

#include "transport_catalogue.h"

void ProcessBusRequest(const trans_cat::TransportCatalogue& tansport_catalogue, 
    const std::string_view& obj_name, std::ostream& output);

void ProcessStopRequest(const trans_cat::TransportCatalogue& tansport_catalogue, 
    const std::string_view& obj_name, std::ostream& output);

void ParseAndPrintStat(const trans_cat::TransportCatalogue& tansport_catalogue, 
    std::string_view request, std::ostream& output);
