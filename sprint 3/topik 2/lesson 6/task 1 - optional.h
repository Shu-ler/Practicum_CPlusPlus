#pragma once

#include <iostream>
#include <optional>
#include <sstream>

template<typename U>
std::ostream& operator << (std::ostream &os, const std::optional<U> &opt) {
    using namespace std::literals;
    if (opt.has_value()) {
        os << *opt;
    } else {
        os << "Значение не задано"s;
    }
    return os;
}

template<typename T>
std::string ToString(T value) {
    std::ostringstream buffer;
    buffer << value;
    return buffer.str();
}
