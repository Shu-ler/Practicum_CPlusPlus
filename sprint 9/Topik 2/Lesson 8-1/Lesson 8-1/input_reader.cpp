#include "input_reader.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <iostream>
#include "stat_reader.h"

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

void InputReader::ApplyCommands([[maybe_unused]] trans_cat::TransportCatalogue& catalogue) const {
    static const std::string stop_cmd("Stop");

    // ������������ ������� ���� "Stop"
    for (auto const& cur : commands_) {
        if (cur.command == stop_cmd) {
            catalogue.AddStop(cur.id, ParseCoordinates(cur.description));
        }
    }

    // ������������ ������� ���� "Bus"
    for (auto const& cur : commands_) {
        if (cur.command != stop_cmd) {
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
