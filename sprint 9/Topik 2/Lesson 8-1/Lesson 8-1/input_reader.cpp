#include "input_reader.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <iostream>
#include "stat_reader.h"

/*
 * ��������� ������ `str` �� ������������ �����, ��������� ���������� ���������.
 * ��������� �������������� ���������� � ���������� � ����������� �� �������� ���������.
 * ���������� ��������� `StopData`, ����������:
 * - ���������� (`coordinates`);
 * - ��������� � ���������� �������� ��������� � ������������ �� ��� (`nearby_stops`).
 *
 * @param str ������ � ������� �� ���������
 * @return ��������� `StopData` � ������������ �������
 * @throws std::invalid_argument ���� ������ ������ ����������� ��� ���������� �� ����� ���� �������������
 */
trans_cat::TransferStopData ParseStopData(std::string str) {
	trans_cat::TransferStopData stop_data;
	std::smatch coords_match;

	// ��������� �������������� ���������
	if (!std::regex_match(str, coords_match, InputReader::coords_regex)) {
		throw std::invalid_argument("Invalid stop data format");
	}

	// ������� ��������� �������������� ���������
	try {
		stop_data.coordinates.lat = std::stod(std::string(coords_match[1].str()));
		stop_data.coordinates.lng = std::stod(std::string(coords_match[2].str()));
	}
	catch (...) {
		throw std::invalid_argument("Invalid coordinates in stop data");
	}

	// ������� ������ - ����� ���������� � ������������ �������� ���������
	std::string remaining_string(coords_match[3].str());

	// ��������� - ������� � ����� ������
	std::sregex_iterator it(remaining_string.begin(), remaining_string.end(), InputReader::distance_regex);
	std::sregex_iterator end;

	// ������� �������� ���������
	for (; it != end; ++it) {
		std::smatch match = *it;
		int distance = std::stoi(match[1].str());
		std::string name(match[2].str());
		stop_data.nearby_stops.emplace(std::move(name), distance);
	}

	return stop_data;
}

/*
 * ������� ������� � ������ � ����� ������
 */
std::string_view Trim(std::string_view string) {
	const auto start = string.find_first_not_of(' ');
	if (start == string.npos) {
		return {};
	}
	return string.substr(start, string.find_last_not_of(' ') + 1 - start);
}

/*
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

/*
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

/*
 * ��������� ������ `line` �� ������������ �����, ��������� ���������� ���������.
 * ������� ������������ ����� ������� � ������� ������������ ��������� ������.
 * ���������� ��������� `CommandDescription`, ����������:
 * - �������� ������� (`command`);
 * - ������������� (`id`);
 * - �������� ���������� (`description`).
 *
 * @param line ������ � ��������� �������
 * @return ��������� `CommandDescription` � ������������ �������
 */
CommandDescription ParseCommandDescription(std::string_view line) {
	CommandDescription result{};
	std::smatch match;

	// ����������� string_view � string
	std::string line_str(line);

	for (const auto& cmd_reg : InputReader::cmd_regs) {
		if (std::regex_match(line_str, match, cmd_reg.first)) {
			result.command = cmd_reg.second;
			result.id = match[1].str();
			result.description = match[2].str();
			break;
		}
	}

	return result;
}

void InputReader::ParseLine(std::string_view line) {
	auto command_description = ParseCommandDescription(line);
	if (command_description) {
		commands_.push_back(std::move(command_description));
	}
}

void InputReader::ApplyCommands([[maybe_unused]] trans_cat::TransportCatalogue& catalogue) const {
	static const std::string stop_cmd("Stop");

	for (auto const& cur : commands_) {

		// ������������ ������� ���� "Stop"
		if (cur.command == stop_cmd) {
			catalogue.AddStop(cur.id, ParseStopData(cur.description));
		}
		
		// ������������ ������� ���� "Bus"
		else if (cur.command != stop_cmd) {
			catalogue.AddRoute(cur.id, ParseRoute(cur.description));
		}
	}
}

void InputReader::LoadCatalog(std::istream& input, trans_cat::TransportCatalogue& catalogue) {
	int base_request_count;
	input >> base_request_count >> std::ws;

	for (int i = 0; i < base_request_count; ++i) {
		std::string line;
		std::getline(input, line);
		ParseLine(line);
	}
	ApplyCommands(catalogue);
}

void InputReader::ProcessRequest(std::istream& input, std::ostream& output, trans_cat::TransportCatalogue& catalogue) {
	int stat_request_count;
	input >> stat_request_count >> std::ws;

	for (int i = 0; i < stat_request_count; ++i) {
		std::string line;
		std::getline(input, line);
		stat_r::ParseAndPrintStat(catalogue, line, output);
	}
}
