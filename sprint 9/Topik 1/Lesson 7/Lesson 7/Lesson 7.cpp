#include <iostream>
#include <string>
#include <unordered_set>

using namespace std;

template <typename Hash>
int FindCollisions(const Hash& hasher, istream& text) {
    std::unordered_set<std::string> words;
    std::unordered_set<size_t> hashs;
    int coll_cnt = 0;
    std::string buf{};

    while (text >> buf) {
        if (!buf.empty() && words.count(buf) == 0) {
            words.insert(buf);
            auto hash = hasher(buf);
            if (hashs.count(hash) == 0) {
                hashs.insert(hash);
            }
            else {
                ++coll_cnt;
            }
        }
    }

    return coll_cnt;
}

// Это плохой хешер. Его можно использовать для тестирования.
// Подумайте, в чём его недостаток
struct HasherDummy {
    size_t operator() (const string& str) const {
        size_t res = 0;
        for (char c : str) {
            res += static_cast<size_t>(c);
        }
        return res;
    }
};

int main() {
    hash<string> str_hasher;
    int collisions = FindCollisions(str_hasher, cin);
    cout << "Found collisions: "s << collisions << endl;
}
