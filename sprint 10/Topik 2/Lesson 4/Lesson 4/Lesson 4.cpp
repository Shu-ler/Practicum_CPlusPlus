#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>

using namespace std;


// clang-format off

const string_view PLANETS[] = {
    "Mercury"sv, "Venus"sv, "Earth"sv,
    "Mars"sv, "Jupiter"sv, "Saturn"sv,
    "Uranus"sv, "Neptune"sv,
};

// clang-format on

bool IsPlanet(string_view name) {
    return ranges::find(PLANETS, name) != end(PLANETS);
}

void Test(string_view name) {
    cout << name << " is " << (IsPlanet(name) ? ""sv : "NOT "sv)
        << "a planet"sv << endl;
}

int main() {
    std::string name;
    std::getline(std::cin, name);
    Test(name);
}
