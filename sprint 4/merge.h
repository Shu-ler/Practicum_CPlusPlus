#pragma once
#include <vector>

// Реализуйте эту функцию.
inline std::vector<int> Merge(const std::vector<int>& arr1, const std::vector<int>& arr2) {
    const size_t arr1_size = arr1.size();
    const size_t arr2_size = arr2.size();
    size_t arr1_index = 0;
    size_t arr2_index = 0;

    std::vector<int> res;
    res.reserve(arr1_size + arr2_size);

    while (arr1_index < arr1_size && arr2_index < arr2_size) {
        if(arr1[arr1_index] < arr2[arr2_index]) {
            res.push_back(arr1[arr1_index++]);
        } else {
            res.push_back(arr2[arr2_index++]);
        }
    }

    while (arr1_index < arr1_size) {
        res.push_back(arr1[arr1_index++]);
    }

    while (arr2_index < arr2_size) {
        res.push_back(arr2[arr2_index++]);
    }

    return res;
}

inline std::vector<int> MergeSort(std::vector<int> arr) {
    if (arr.size() <= 1) {
        return arr;
    }
    const size_t mid = arr.size() / 2;
    const std::vector<int> left_sorted = MergeSort({arr.begin(), arr.begin() + mid});
    const std::vector<int> right_sorted = MergeSort({arr.begin() + mid, arr.end()});

    return Merge(left_sorted, right_sorted);
}
