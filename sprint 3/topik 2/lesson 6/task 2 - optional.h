#pragma once

#include <iostream>
#include <optional>
#include <sstream>

template<typename U>
std::istream& operator>>(std::istream& is, std::optional<U>& opt) {
    U value;
    if (is >> value) {
        opt = value;
    }
    else {
        is.clear();
        opt = std::nullopt;
    }
    return is;
}

template <typename T>
std::optional<T> FromString(std::string str) {
    std::istringstream is(str);
    std::optional<T> opt;
    is >> opt;
    return opt;
}
