#include "input_reader.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <iostream>

/**
 * ������ ������ ���� "10.123,  -30.1837" � ���������� ���� ��������� (������, �������)
 */
Coordinates ParseCoordinates(std::string_view str) {
	static const double nan = std::nan("");

	auto not_space = str.find_first_not_of(' ');
	auto comma = str.find(',');

	if (comma == str.npos) {
		return { nan, nan };
	}

	auto not_space2 = str.find_first_not_of(' ', comma + 1);

	double lat = std::stod(std::string(str.substr(not_space, comma - not_space)));
	double lng = std::stod(std::string(str.substr(not_space2)));

	return { lat, lng };
}

/**
 * ������� ������� � ������ � ����� ������
 */
std::string_view Trim(std::string_view string) {
	const auto start = string.find_first_not_of(' ');
	if (start == string.npos) {
		return {};
	}
	return string.substr(start, string.find_last_not_of(' ') + 1 - start);
}

/**
 * ��������� ������ string �� n �����, � ������� ���������� �������-����������� delim
 */
std::vector<std::string_view> Split(std::string_view string, char delim) {
	std::vector<std::string_view> result;

	size_t pos = 0;
	while ((pos = string.find_first_not_of(' ', pos)) < string.length()) {
		auto delim_pos = string.find(delim, pos);
		if (delim_pos == string.npos) {
			delim_pos = string.size();
		}
		if (auto substr = Trim(string.substr(pos, delim_pos - pos)); !substr.empty()) {
			result.push_back(substr);
		}
		pos = delim_pos + 1;
	}

	return result;
}

/**
 * ������ �������.
 * ��� ���������� �������� (A>B>C>A) ���������� ������ �������� ��������� [A,B,C,A]
 * ��� ������������ �������� (A-B-C-D) ���������� ������ �������� ��������� [A,B,C,D,C,B,A]
 */
std::vector<std::string_view> ParseRoute(std::string_view route) {
	if (route.find('>') != route.npos) {
		return Split(route, '>');
	}

	auto stops = Split(route, '-');
	std::vector<std::string_view> results(stops.begin(), stops.end());
	results.insert(results.end(), std::next(stops.rbegin()), stops.rend());

	return results;
}

CommandDescription ParseCommandDescription(std::string_view line) {
	auto colon_pos = line.find(':');
	if (colon_pos == line.npos) {
		return {};
	}

	auto space_pos = line.find(' ');
	if (space_pos >= colon_pos) {
		return {};
	}

	auto not_space = line.find_first_not_of(' ', space_pos);
	if (not_space >= colon_pos) {
		return {};
	}

	return { std::string(line.substr(0, space_pos)),
			std::string(line.substr(not_space, colon_pos - not_space)),
			std::string(line.substr(colon_pos + 1)) };
}

void InputReader::ParseLine(std::string_view line) {
	auto command_description = ParseCommandDescription(line);
	if (command_description) {
		commands_.push_back(std::move(command_description));
	}
}

/**
 * ���������� ����������� �������� ���������
 * ��� ������������ ������ ���������� '*'
 */
char RouteStopDivider(const std::string& description) {
	char divider = route::Route::IsOrdinaryRoute(description)
		? '-'
		: route::Route::IsRingRoute(description)
		? '>'
		: '*';
	return divider;
}

/**
 * ������ ���������.
 * ���������� ������ ���������� �� ���������
 * ��� �������������� ��������� ������ ��������� � �������� ������������
 */
RouteStops ParseStops(const std::string& description, trans_catalogue::TransportCatalogue& catalogue) {
	RouteStops stops;
	char delim = RouteStopDivider(description);
	
	if (delim != '*') {
		std::vector<std::string_view> elements = Split(description, delim);
		for (auto el : elements) {
			auto stops_it = catalogue.FindStop(el);

			// ��� �������������� ��������� ������ ��������� � �������� ������������
			if (stops_it == nullptr) {
				std::string stop_name{ el };
				catalogue.AddStop(stop_name, { 0,0 });
				stops_it = catalogue.FindStop(el);
			}

			// ��������� ��������� � ������
			stops.emplace_back(stops_it); 
		}
	}
	return stops;
}

void InputReader::ApplyCommands([[maybe_unused]] trans_catalogue::TransportCatalogue& catalogue) const {
	auto slider = commands_.begin();			// ������� �� ������� ������
	std::string stop_cmd("Stop");	// ����������� ������ "Stop"

	// ��������� ������� �� ����� - "Stop" ������
	std::sort(commands_.begin(), commands_.end(), [](const CommandDescription& a, const CommandDescription& b) {
		return a.command < b.command;
		});

	// ������� upper_bound ��� "Stop"
	auto first_bus = std::upper_bound(commands_.begin(), commands_.end(), stop_cmd,
		[](const CommandDescription& cmd, const std::string& value) {
			return cmd.command < value;
		});

	// ������������ ������� ���� "Stop"
	for (; slider < first_bus; ++slider) {
		catalogue.AddStop(slider->id, ParseCoordinates(slider->description));
	}

	// ������������ ������� ���� "Bus"
	for (; slider < commands_.end(); ++slider) {
		RouteStops stops = ParseStops(slider->description, catalogue);
		catalogue.AddRoute(slider->id, stops);
	}
}

