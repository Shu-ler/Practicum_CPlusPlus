#include <cassert>
#include <vector>

template <typename T>
void ReverseArray(T* start, size_t size) {
    if (size > 1) {
        T buf{};
        auto left = start;
        auto right = start + size - 1;
        while (left < right) {
            buf = *left;
            *left = *right;
            *right = buf;
            ++left;
            --right;
        }
    }
}

int main() {
    using namespace std;

    vector<int> v = { 1, 2 };
    ReverseArray(v.data(), v.size());
    assert(v == (vector<int>{2, 1}));
}
