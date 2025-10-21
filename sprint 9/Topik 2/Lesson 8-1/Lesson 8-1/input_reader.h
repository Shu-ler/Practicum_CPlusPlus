#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <regex>

#include "geo.h"
#include "transport_catalogue.h"

/**
 * ��������� CommandDescription - �������� ������ ����������� �������
 */
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

/**
 * ����� InputReader - �������� �� ������ � ��������� ������� ������ ��� ������������� �����������
 */
class InputReader {
public:

	// ������� ���������� ��������� ��� �������� ������
	inline static const std::vector<std::pair<std::regex, std::string>> cmd_regs{
		std::pair(std::regex(R"(^Stop ([^\s:]+(?: [^\s:]*)*): (.*)$)"), "Stop"),
		std::pair(std::regex(R"(^Bus ([^\s:]+): (.*)$)"), "Bus") };

	/**
	 * ���������� ������������� ����������� �� ���������� ������
	 */
	void LoadCatalog(std::istream& input, trans_cat::TransportCatalogue& catalogue);

	/**
	 * ���������� �������� � ������������� ����������� �� ���������� ������ � ����� � ��������� �����
	 */
	void ProcessRequest(std::istream& input, std::ostream& output, trans_cat::TransportCatalogue& catalogue);

private:
	/**
	 * ������ ������ � ��������� CommandDescription � ��������� ��������� � commands_
	 */
	void ParseLine(std::string_view line);

	/**
	 * ��������� ������� ������������ ����������, ��������� ������� �� commands_
	 */
	void ApplyCommands(trans_cat::TransportCatalogue& catalogue) const;

private:
	// ��������� �������� ������ (��������) ����������� ������
	std::vector<CommandDescription> commands_;
};
