#include "ini.h"

using namespace std;

namespace ini {

	Section& ini::Document::AddSection(std::string name) {
		// TODO: �������� ����� �������� return
	}

	const Section& Document::GetSection(const std::string& name) const {
		// TODO: �������� ����� �������� return
	}

	std::size_t Document::GetSectionCount() const {
		return std::size_t();
	}

	Document Load(std::istream& input)
	{
		return Document();
	}

} // namespace ini
