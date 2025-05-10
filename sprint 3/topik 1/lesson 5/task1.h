#pragma once

#include <iostream>
#include <sstream>

using namespace std::literals;

// Напишите здесь перегрузки операций.
inline std::ostream& operator<<(std::ostream& os, const std::pair<int, int>& p) {
    os << "("s << p.first << ", "s << p.second << ")"s;
    return os;
};

inline std::istream& operator>>(std::istream& is, std::pair<int, int>& p) {
    int first, second;
    if (is >> first >> second) {
        p.first = first;
        p.second = second;
    };
//    is >> p.first >> p.second;
    return is;
};

// Функция для вывода значения в строку.
inline std::string ToString(std::pair<int, int> p) {
    std::ostringstream buffer;
    buffer << p;
    return buffer.str();
}

// Функция для записи значения в строку.
inline std::pair<int, int> FromString(std::string str) {
    std::istringstream is(str);
    std::pair<int, int> p;
    if (is >> p) {
        return p;
    }
    return {0, 0};
}