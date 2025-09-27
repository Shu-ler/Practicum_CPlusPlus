#include "ini.h"
#include <assert.h>

using namespace std;

namespace ini {

	Section& ini::Document::AddSection(std::string name) {
		assert(sections_.count(name) == 0); // ��������, ��� ������ � ����� ������ ��� �� ����������
		return sections_[name];				// ��������� ����� ������ � ���������� ������ �� ��
	}

	const Section& Document::GetSection(const std::string& name) const {
		static const Section blank; // ������ ������ ������ Section ��� �������� � ������ ���������� ������

		if (sections_.count(name) == 0) {
			// ���� ������ �� �������, ���������� ������ ������
			return blank;
		}

		return sections_.at(name); // ���� ������ �������, ���������� �
	}

	std::size_t Document::GetSectionCount() const {
		return sections_.size();
	}

	Document Load(std::istream& input) {
		Document doc;
		std::string buffer{};
		std::string clear_buffer{};
		Section* cur_section = nullptr;

		while (std::getline(input, buffer)) {
			if (!StringEmpty(buffer)) {
				clear_buffer = StringTrim(buffer);
				if (clear_buffer[0] == '[') {
					cur_section = &doc.AddSection(clear_buffer.substr(1, clear_buffer.size() - 2));
				}
				else if (cur_section != nullptr) {
					size_t eq_pos = clear_buffer.find('=');
					if (eq_pos != std::string::npos) {
						std::string key = StringTrim(clear_buffer.substr(0, eq_pos));
						std::string value = StringTrim(clear_buffer.substr(eq_pos + 1));
						(*cur_section)[key] = value;
					}
				}
			}
		}
		return doc;
	}

	bool StringEmpty(const std::string& name) {
		return name.find_first_not_of(" \t\n\v\f\r") == std::string::npos;
	}

	std::string StringTrim(const std::string& str) {
		static const std::string spaces = " \t\n\v\f\r";

		// ����� ������� ������������� �������
		size_t first = str.find_first_not_of(spaces);
		if (first != std::string::npos) {

			// ����� ���������� ������������� �������
			size_t last = str.find_last_not_of(spaces);
			return str.substr(first, last - first + 1);

		}
		else {
			return string();
		}
	}

} // namespace ini
