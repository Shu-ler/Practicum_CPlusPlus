#include <iostream>
#include <string>
#include <string_view>

using namespace std;


// clang-format off

const string_view PLANETS[] = {
    "Mercury"sv, "Venus"sv, "Earth"sv,
    "Mars"sv, "Jupiter"sv, "Saturn"sv,
    "Uranus"sv, "Neptune"sv,
};

// clang-format on

bool IsPlanet(string_view name) {
    for (auto const& Planet : PLANETS) {
        if (Planet == name) {
            return true;
        }
    }
    return false;
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
