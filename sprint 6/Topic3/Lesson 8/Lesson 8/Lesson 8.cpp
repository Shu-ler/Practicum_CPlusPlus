#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using filesystem::path;

enum class IncludeType {
	None,
	WithRoot,
	OnlySystem,
};

struct IncludeFile {
	bool include_find = false;
	IncludeType inc_type = IncludeType::None;
	string inc_file_name{};
	bool file_exist = false;
	path include_file_path;
	ifstream inc_stream;
};

path operator""_p(const char* data, std::size_t sz) {
	return path(data, data + sz);
}

bool Preprocess(const path& in_file, const path& out_file, const vector<path>& include_directories);
bool DoPreprocess(ifstream& src_file, const path& in_file, ofstream& dst_file, const vector<path>& include_directories);
bool LineContainsInclude(const string& line, const regex& reg);
IncludeFile ContainsInclude(const string& line);
bool GetInclFile(IncludeFile& inc_file, const path& in_file, const vector<path>& include_directories);

IncludeFile GetIncludeFile(const string& line, const regex& reg, const vector<path>& include_directories, const path& cur_path, bool only_systems = true);

static void ErrorMsg(const string& src_file_name, const string& dst_file_name, size_t str_num) {
	std::cout << "unknown include file "s
		<< dst_file_name
		<< " at file "s
		<< src_file_name
		<< " at line "s
		<< str_num
		<< endl;
}

string GetFileContents(string file) {
	ifstream stream(file);

	// конструируем string по двум итераторам
	return { (istreambuf_iterator<char>(stream)), istreambuf_iterator<char>() };
}

void Test() {
	error_code err;
	filesystem::remove_all("sources"_p, err);
	filesystem::create_directories("sources"_p / "include2"_p / "lib"_p, err);
	filesystem::create_directories("sources"_p / "include1"_p, err);
	filesystem::create_directories("sources"_p / "dir1"_p / "subdir"_p, err);

	{
		ofstream file("sources/a.cpp");
		file << "// this comment before include\n"
			"#include \"dir1/b.h\"\n"
			"// text between b.h and c.h\n"
			"#include \"dir1/d.h\"\n"
			"\n"
			"int SayHello() {\n"
			"    cout << \"hello, world!\" << endl;\n"
			"#   include<dummy.txt>\n"
			"}\n"s;
	}
	{
		ofstream file("sources/dir1/b.h");
		file << "// text from b.h before include\n"
			"#include \"subdir/c.h\"\n"
			"// text from b.h after include"s;
	}
	{
		ofstream file("sources/dir1/subdir/c.h");
		file << "// text from c.h before include\n"
			"#include <std1.h>\n"
			"// text from c.h after include\n"s;
	}
	{
		ofstream file("sources/dir1/d.h");
		file << "// text from d.h before include\n"
			"#include \"lib/std2.h\"\n"
			"// text from d.h after include\n"s;
	}
	{
		ofstream file("sources/include1/std1.h");
		file << "// std1\n"s;
	}
	{
		ofstream file("sources/include2/lib/std2.h");
		file << "// std2\n"s;
	}

	assert((!Preprocess("sources"_p / "a.cpp"_p, "sources"_p / "a.in"_p,
		{ "sources"_p / "include1"_p,"sources"_p / "include2"_p })));

	ostringstream test_out;
	test_out << "// this comment before include\n"
		"// text from b.h before include\n"
		"// text from c.h before include\n"
		"// std1\n"
		"// text from c.h after include\n"
		"// text from b.h after include\n"
		"// text between b.h and c.h\n"
		"// text from d.h before include\n"
		"// std2\n"
		"// text from d.h after include\n"
		"\n"
		"int SayHello() {\n"
		"    cout << \"hello, world!\" << endl;\n"s;

	auto aa = GetFileContents("sources/a.in"s);
	assert(GetFileContents("sources/a.in"s) == test_out.str());
}

int main() {
	Test();
}

bool Preprocess(const path& in_file, const path& out_file, const vector<path>& include_directories) {
	bool success = false;
	ifstream src_file(in_file);

	if (static_cast<bool>(src_file)) {
		ofstream dst_file(out_file);
		if (static_cast<bool>(dst_file)) {
			success = DoPreprocess(src_file, in_file, dst_file, include_directories);
		}
	}

	return success;
}

bool DoPreprocess(ifstream& src_file, const path& in_file, ofstream& dst_file, const vector<path>& include_directories) {
	static const regex reg1(R"/(\s*#\s*include\s*"([^"]*)"\s*)/");
	static const regex reg2(R"/(\s*#\s*include\s*<([^>]*)>\s*)/");

	bool success = true;

	size_t line_num = 0;
	string line;
	path inc_path;

	while (getline(src_file, line) && success) {
		++line_num;

		IncludeFile inc_file = ContainsInclude(line);

		if (inc_file.inc_type == IncludeType::None) {
			dst_file << line << endl;
		}
		else {
			if (GetInclFile(inc_file, in_file, include_directories)) {
				success = DoPreprocess(inc_file.inc_stream, inc_file.include_file_path, dst_file, include_directories);
			}
			else {
				ErrorMsg(in_file.filename().string(), inc_path.filename().string(), line_num);
				success = false;
			}
		}
	}

	return success;
}

bool LineContainsInclude(const string& line, const regex& reg) {
	return regex_search(line, reg);
}

IncludeFile ContainsInclude(const string& line) {
	IncludeFile res;

	static const regex reg1(R"/(\s*#\s*include\s*"([^"]*)"\s*)/");
	static const regex reg2(R"/(\s*#\s*include\s*<([^>]*)>\s*)/");

	smatch match;
	string include_file_name;

	if (regex_search(line, match, reg1)) {
		res.inc_file_name = match[1];
		res.include_find = true;
		res.inc_type = IncludeType::WithRoot;
	} 
	else if (regex_search(line, match, reg2)) {
		res.inc_file_name = match[1];
		res.include_find = true;
		res.inc_type = IncludeType::OnlySystem;
	}

	return res;
}

bool GetInclFile(IncludeFile& inc_file, const path& in_file, const vector<path>& include_directories) {
	bool res = false;

	// Обработка инклюдов вида "..."
	if (inc_file.inc_type == IncludeType::WithRoot) {  
		auto cand_path = in_file.parent_path(); // file_name;
		if (filesystem::exists(cand_path)) {
			inc_file.inc_stream = std::ifstream(cand_path);
			inc_file.file_exist = static_cast<bool>(inc_file.inc_stream);
		}
	}

	return false;
}

bool GetIncludePath(const string& line, const regex& reg, path& inc_path, const path& cur_path) {
	bool res = false;
	smatch match;
	string include_file_name;

	if (regex_search(line, match, reg)) {
		include_file_name = match[1];
		inc_path = cur_path.parent_path() / include_file_name;
		res = true;
	}

	return res;
}

IncludeFile GetIncludeFile(const string& line, const regex& reg, const vector<path>& include_directories, const path& cur_path, bool only_systems) {
	IncludeFile res;
	smatch match;
	string include_file_name;

	if (regex_search(line, match, reg)) {
		include_file_name = match[1];
		res.include_file_path = cur_path.parent_path() / include_file_name;
		res.include_find = true;
	}

//	auto file_name = in_file.filename();

//	if (!only_systems) {  // Обработка инклюдов вида "..."
//		auto cand_path = in_file.parent_path() / file_name;
//		if (filesystem::exists(cand_path)) {
//			res.inc_file = ifstream(cand_path);
//			res.exist = static_cast<bool>(res.inc_file);
//		}
//	}

//	if (!res.exist) {
//		for (const auto std_dir : include_directories) {
//			auto cand_path = std_dir / file_name;
//			if (filesystem::exists(cand_path)) {
//				res.inc_file = ifstream(cand_path);
//				res.exist = static_cast<bool>(res.inc_file);
//				break;
//			}
//		}
//	}

	return res;
}
