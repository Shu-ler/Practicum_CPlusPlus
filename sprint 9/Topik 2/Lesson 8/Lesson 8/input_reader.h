#pragma once
#include <string>
#include <string_view>
#include <vector>

#include "geo.h"
#include "transport_catalogue.h"

using StopPtr = const stop::Stop*;
using RoutePtr = const route::Route*;
using RouteStops = std::vector<StopPtr>;

struct CommandDescription {
    // ����������, ������ �� ������� (���� command ��������)
    explicit operator bool() const {
        return !command.empty();
    }

    bool operator!() const {
        return !operator bool();
    }

    std::string command;      // �������� �������
    std::string id;           // id �������� ��� ���������
    std::string description;  // ��������� �������
};

//bool operator<(const CommandDescription& lhs, const CommandDescription& rhs) {
//    return lhs.command < rhs.command; // ���������� �� ���� command
//}
//
//bool operator<(const CommandDescription& lhs, const std::string& rhs) {
//    return lhs.command < rhs; // ��������� ���� command �� �������
//}

class InputReader {
public:
    /**
     * ������ ������ � ��������� CommandDescription � ��������� ��������� � commands_
     */
    void ParseLine(std::string_view line);

    /**
     * ��������� ������� ������������ ����������, ��������� ������� �� commands_
     */
    void ApplyCommands(trans_catalogue::TransportCatalogue& catalogue);

private:
    std::vector<CommandDescription> commands_;
};
