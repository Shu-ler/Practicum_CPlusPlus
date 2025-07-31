#pragma once

#include <vector>
#include <set>

template<class T>
size_t CountUniqueValues(std::vector<T> source) {
    auto buffer = std::set<T>(source.begin(), source.end());
    return buffer.size();
}

