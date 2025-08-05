#pragma once

#include <iostream>
#include <map>
#include <optional>

template<typename K, typename V>
class BiMap {
public:
    BiMap() = default;
    BiMap(const std::map<K, V>& input_map) {
        for (const auto in_pair : input_map) {
            Insert(in_pair);
        }
    }

    void Insert(const std::pair<K, V> item);
    std::optional<V> GetByKey(const K& key);
    std::optional<K> GetByValue(const V& value);
private:
    std::map<K, V> forward_;    // Словарь <ключ, значение>
    std::map<V, K> backward_;   // Словарь <значение, ключ>
};

template<typename K, typename V>
inline void BiMap<K, V>::Insert(const std::pair<K, V> item) {

    // Мой вариант
    // forward_.insert(item);
    // backward_.insert({item.second, item.first});

    // Из авторского решения
    forward_[item.first] = item.second;
    backward_[item.second] = item.first;
}

template<typename K, typename V>
inline std::optional<V> BiMap<K, V>::GetByKey(const K &key) {
    auto iterator = forward_.find(key);
    if (iterator == forward_.end()) {
        return std::nullopt;
    } else {
        return iterator->second;
    }
}

template<typename K, typename V>
inline std::optional<K> BiMap<K, V>::GetByValue(const V &value) {
    auto iterator = backward_.find(value);
    if (iterator == backward_.end()) {
        return std::nullopt;
    } else {
        return iterator->second;
    }
}
