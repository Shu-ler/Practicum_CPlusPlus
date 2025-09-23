#include "ini.h"

using namespace std;

namespace ini {

	Section& ini::Document::AddSection(std::string name) {
		// TODO: вставьте здесь оператор return
	}

	const Section& Document::GetSection(const std::string& name) const {
		// TODO: вставьте здесь оператор return
	}

	std::size_t Document::GetSectionCount() const {
		return std::size_t();
	}

	Document Load(std::istream& input)
	{
		return Document();
	}

} // namespace ini
