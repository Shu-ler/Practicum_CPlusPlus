#pragma once

#include  <utility>

template<typename T>
inline std::pair<T, T> Swap(const std::pair<T, T>& src){
    std::pair<T, T> buf{src.second, src.first};
    return buf;
}
