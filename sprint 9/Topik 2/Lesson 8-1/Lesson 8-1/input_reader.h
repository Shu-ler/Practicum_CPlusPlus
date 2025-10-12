#pragma once
#include <string>
#include <string_view>
#include <vector>

#include "geo.h"
#include "transport_catalogue.h"

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

class InputReader {
public:
    /**
     * ������ ������ � ��������� CommandDescription � ��������� ��������� � commands_
     */
    void ParseLine(std::string_view line);

    /**
     * ��������� ������� ������������ ����������, ��������� ������� �� commands_
     */
    void ApplyCommands(trans_cat::TransportCatalogue& catalogue) const;

    /**
     * ���������� ����������� �������� ���������
     * ��� ������������ ������ ���������� '*'
     */
    static char RouteStopDivider(const std::string_view description);

private:
    static bool IsOrdinaryRoute(const std::string_view description);
    static bool IsRingRoute(const std::string_view description);

private:
    std::vector<CommandDescription> commands_;
};
