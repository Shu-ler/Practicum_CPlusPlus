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

class ProcessFile
{
public:
	ProcessFile(const path& in_file, IncludeType new_type, const vector<path>& include_directories);
	ProcessFile(const path& in_file);
	~ProcessFile() {};

	bool IsOk();
	bool IsOutstreamOk();

	bool ConnectOutStream(const path& out_file);
	ofstream& GetOutStream();
	bool DoPreprocess(ofstream& dst_stream, const vector<path>& include_directories);
	bool ContainsInclude(const string& line);

	void ErrorMsg(const string& dst_file_name, size_t str_num);

private:
	path file_path_;
	ifstream inc_stream_;
	ifstream src_stream_;
	ofstream dst_stream_;

	IncludeType inc_type_ = IncludeType::None;
	string inc_file_name_{};
};

ProcessFile::ProcessFile(const path& in_file, IncludeType new_type, const vector<path>& include_directories)  {
	bool res = false;

	// Обработка инклюдов вида "..."
	if (new_type == IncludeType::WithRoot) {
		auto cand_path = in_file.parent_path() / in_file;
		if (filesystem::exists(cand_path)) {
			src_stream_ = std::ifstream(cand_path);
		}
	}

	if (!IsOk()) {
		for (const auto std_dir : include_directories) {
			auto cand_path = std_dir / in_file;
			if (filesystem::exists(cand_path)) {
				src_stream_ = std::ifstream(cand_path);
				if (IsOk()) {
					break;
				}
			}
		}
	}
}

ProcessFile::ProcessFile(const path& in_file) : file_path_(in_file) {
	src_stream_.open(file_path_);
}

bool ProcessFile::IsOk() {
	return static_cast<bool>(src_stream_);
}

bool ProcessFile::IsOutstreamOk() {
	return static_cast<bool>(dst_stream_);
}

bool ProcessFile::ConnectOutStream(const path& out_file) {
	dst_stream_.open(out_file);
	return IsOutstreamOk();
}

ofstream& ProcessFile::GetOutStream() {
	return dst_stream_;
}

bool ProcessFile::DoPreprocess(ofstream& dst_stream, const vector<path>& include_directories) {
	bool success = true;

	size_t line_num = 0;
	string line;
	path inc_path;

	while (getline(src_stream_, line) && success) {
		++line_num;

		if (ContainsInclude(line)) {
			ProcessFile sub_file(path(inc_file_name_), inc_type_, include_directories);
			if (sub_file.IsOk()) {
				success = sub_file.DoPreprocess(dst_stream, include_directories);
			}
			else {
				ErrorMsg("777", line_num);
			}
		}
		else {
			dst_stream << line << endl;
		}
	}

	return success;
}

bool ProcessFile::ContainsInclude(const string& line) {
	static const vector<pair<regex, IncludeType>> regs{ pair(regex(R"/(\s*#\s*include\s*"([^"]*)"\s*)/"), IncludeType::WithRoot),
														pair(regex(R"/(\s*#\s*include\s*<([^>]*)>\s*)/"), IncludeType::OnlySystem) };
	smatch m;

	inc_type_ = IncludeType::None;

	for (const auto& reg : regs) {
		if (regex_search(line, m, reg.first)) {
			inc_file_name_ = m[1];
			inc_type_ = reg.second;
			break;
		}
	}
	return inc_type_ != IncludeType::None;
}

void ProcessFile::ErrorMsg(const string& dst_file_name, size_t str_num) {
	std::cout << "unknown include file "s
		<< dst_file_name
		<< " at file "s
		<< file_path_.filename().string()
		<< " at line "s
		<< str_num
		<< endl;
}

path operator""_p(const char* data, std::size_t sz) {
	return path(data, data + sz);
}

bool Preprocess(const path& in_file, const path& out_file, const vector<path>& include_directories);

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
	ProcessFile src_f(in_file);
	bool success = src_f.IsOk();

	if (success && src_f.ConnectOutStream(out_file)) {
		success = src_f.DoPreprocess(src_f.GetOutStream(), include_directories);
	}

	return success;
}

