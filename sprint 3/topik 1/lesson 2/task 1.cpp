#include <vector>
#include <iostream>
#include <utility>
#include <string>

using namespace std::literals;

// Названия дней недели на английском.
static const std::vector<std::string> WEEKDAY_NAMES = {
    "Monday"s,
    "Tuesday"s,
    "Wednesday"s,
    "Thursday"s,
    "Friday"s,
    "Saturday"s,
    "Sunday"s,
    "Unknown"s
};

enum class Weekday {
    MONDAY = 0,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY,
    UNKNOWN
};

// Напишите эти функции.
Weekday Add(Weekday day, int value) {
    int N = 7;
    int new_day = 7;

    if (day != Weekday::UNKNOWN) {
        new_day = (static_cast<int>(day) + value);
        new_day = (new_day % N + N) % N;
    }
    return static_cast<Weekday>(new_day);
};

Weekday Sub(Weekday day, int value) {
    int N = 7;
    int new_day = 7;

    if (day != Weekday::UNKNOWN) {
        new_day = (static_cast<int>(day) - value);
        new_day = (new_day % N + N) % N;
    }
    return static_cast<Weekday>(new_day);
};

void Print(std::ostream& out, Weekday day) {
        out << WEEKDAY_NAMES[static_cast<int>(day)];
};

Weekday Read(std::istream& inp) {
    std::string buffer = "";
    size_t i = 0;

    inp >> buffer;

    for (; i <= WEEKDAY_NAMES.size(); ++i) {
        if (buffer == WEEKDAY_NAMES[i]) {
            return static_cast<Weekday>(i);
        }
    }
       
    return Weekday::UNKNOWN;        
};

int main() {
    int val_add = 0;
    int val_sub = 0;

    Weekday init_weekday = Read(std::cin);

    std::cin >> val_add;
    std::cin >> val_sub;

    std::cout << val_add << " day(s) after it will be ";
    Print(std::cout, Add(init_weekday, val_add));
    std::cout << std::endl;

    std::cout << val_sub << " day(s) before it was ";
    Print(std::cout, Sub(init_weekday, val_sub));
    std::cout << std::endl;
}
