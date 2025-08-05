#pragma once

#include <vector>
#include <set>

template<class T>
std::vector<T> FindDuplicates(std::vector<T>& source) {
    std::set<T> unique_values;
    std::set<T> duplicate_values;

    for (const auto& src : source) {
        if (unique_values.contains(src)) {
            duplicate_values.insert(src);
        } else {
            unique_values.insert(src);
        }
    }

    return std::vector<T>(duplicate_values.begin(), duplicate_values.end());
}

template<class T>
std::set<T> Intersection(const std::set<T>& set1, const std::set<T>& set2) {
    std::set<T> res;
    for (const auto& set1el : set1) {
        if (set2.find(set1el) != set2.end()) {
            res.insert(set1el);
        }
    }
    return res;
}

template<class T>
std::set<T> Union(const std::set<T>& set1, const std::set<T>& set2) {
    std::set<T> res = set1;
    for (const auto& set2el : set2) {
        if (res.find(set2el) == res.end()) {
            res.insert(set2el);
        }
    }
    return res;
}

template<class T>
std::set<T> Difference(const std::set<T>& set1, const std::set<T>& set2) {
    std::set<T> res;
    for (const auto& curr_el: set1) {
        if (set2.find(curr_el) == set2.end()) {
            res.insert(curr_el);
        }
    }
    return res;
}
