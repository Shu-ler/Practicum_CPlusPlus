#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
using filesystem::path;

path operator""_p(const char* data, std::size_t sz) {
    return path(data, data + sz);
}

void PrintTree(ostream& dst, const path& p, const filesystem::file_status& status, int offset) {
    dst << string(offset, ' ') << p.filename().string() << endl;

    if (status.type() == filesystem::file_type::directory) {
        std::vector<filesystem::directory_entry> all_elements;

        for (const auto& dir_entry : filesystem::directory_iterator(p)) {
            all_elements.push_back(dir_entry);
        }

        std::sort(all_elements.begin(), all_elements.end(), [](const filesystem::directory_entry& lhs, const filesystem::directory_entry& rhs) {
            if (lhs.status().type() == rhs.status().type()) {
                return lhs.path().filename() > rhs.path().filename();
            }
            return (lhs.status().type() == filesystem::file_type::directory); });

        for (const auto& dir_entry : all_elements) {
            PrintTree(dst, dir_entry.path(), dir_entry.status(), offset + 2);
        }
    }
};

// напишите эту функцию
void PrintTree(ostream& dst, const path& p) {
    int offset = 0;
    PrintTree(dst, p, filesystem::status(p), offset);
};

int main() {
    error_code err;
    filesystem::remove_all("test_dir", err);
    filesystem::create_directories("test_dir"_p / "a"_p, err);
    filesystem::create_directories("test_dir"_p / "b"_p, err);

    ofstream("test_dir"_p / "b"_p / "f1.txt"_p);
    ofstream("test_dir"_p / "b"_p / "f2.txt"_p);
    ofstream("test_dir"_p / "a"_p / "f3.txt"_p);

    ostringstream out;
    PrintTree(out, "test_dir"_p);
    auto aa = out.str();
    assert(out.str() ==
        "test_dir\n"
        "  b\n"
        "    f2.txt\n"
        "    f1.txt\n"
        "  a\n"
        "    f3.txt\n"s
    );
}
