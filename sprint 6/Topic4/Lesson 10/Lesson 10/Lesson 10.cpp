#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int EffectiveCount(const vector<int>& v, int n, int i) {
    if (v.empty()) return 0;

    int64_t expected = static_cast<int64_t>(v.size()) * (i + 1) / (n + 1);
    int64_t threshold = static_cast<int64_t>(log2(v.size()));

    if (expected < threshold) {
        auto it = find_if(v.begin(), v.end(), [i](int x) { return x > i; });
        std::cout << "Using find_if\n";
        return static_cast<int>(it - v.begin());
    }
    else {
        auto it = upper_bound(v.begin(), v.end(), i);
        std::cout << "Using upper_bound\n";
        return static_cast<int>(it - v.begin());
    }
}

int main() {
    static const int NUMBERS = 1'000'000;
    static const int MAX = 1'000'000'000;

    mt19937 r;
    uniform_int_distribution<int> uniform_dist(0, MAX);

    vector<int> nums;
    for (int i = 0; i < NUMBERS; ++i) {
        int random_number = uniform_dist(r);
        nums.push_back(random_number);
    }
    sort(nums.begin(), nums.end());

    int i;
    cin >> i;
    int result = EffectiveCount(nums, MAX, i);
    cout << "Total numbers before "s << i << ": "s << result << endl;
}
