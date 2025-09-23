#include <iostream>
#include <string>

using namespace std;

struct Circle {
    double x;
    double y;
    double r;
};

struct Dumbbell {
    Circle circle1;
    Circle circle2;
    string text;
};

struct DumbbellHash {
    size_t operator()(const Dumbbell& dumbbell) const {
        size_t mult = 37;
        size_t hash_res = d_hasher_(dumbbell.circle1.x);

        hash_res += d_hasher_(dumbbell.circle1.y) * mult;
        mult *= 37;
        hash_res += d_hasher_(dumbbell.circle1.r) * mult;
        mult *= 37;

        hash_res += d_hasher_(dumbbell.circle2.x) * mult;
        mult *= 37;
        hash_res += d_hasher_(dumbbell.circle2.y) * mult;
        mult *= 37;
        hash_res += d_hasher_(dumbbell.circle2.r) * mult;
        mult *= 37;

        hash_res += s_hasher_(dumbbell.text) * mult;

        return hash_res;
    }
private:
    std::hash<double> d_hasher_;
    std::hash<std::string> s_hasher_;
};

int main() {
    DumbbellHash hash;
    Dumbbell dumbbell{ {10, 11.5, 2.3}, {3.14, 15, -8}, "abc"s };
    cout << "Dumbbell hash "s << hash(dumbbell);
}
