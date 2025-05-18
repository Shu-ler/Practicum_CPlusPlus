#pragma once

#include <optional>
#include <tuple>
#include <vector>

template<typename T>
std::optional<std::tuple<T, int, T, int>> FindMinMax(const std::vector<T>& values) {
    if (values.size() == 0) {
        return std::nullopt;
    }
    else {
        int min_index = 0;
        T min = values[0];
        int max_index = 0;
        T max = values[0];

        for (size_t i = 1; i < values.size(); i++) {
            if (values[i] > max) {
                max = values[i];
                max_index = i;
            } else if (values[i] < min) {
                min = values[i];
                min_index = i;
            }
        }
        return std::tuple(min, min_index, max, max_index);
    }
}
