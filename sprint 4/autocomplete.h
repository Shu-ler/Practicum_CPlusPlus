#pragma once

#include <set>
#include <string>

class AutocompleteEngine {
private:
    using Storage = std::set<std::string>;

public:
    using It = Storage::const_iterator;

public:
    AutocompleteEngine() {
    }

    // Следующие два метода нужно реализовать.
    void AddName(const std::string& name) {
        names_.insert(name);
    }

std::pair<It, It> Autocomplete(const std::string& input, size_t max_names) const {
    const auto from = names_.lower_bound(input);
    // Определяем верхнюю границу для поиска имён, начинающихся с input
    const auto max_to = names_.upper_bound(input + '{'); // '{' — следующий символ после 'z'
    auto to = from;

    // Проходим по элементам, пока не достигнем max_names или конца диапазона
    while (to != max_to && max_names != 0) {
        ++to;
        max_names -= 1;
    }
    return {from, to};
}

private:
    // Тут должен быть Storage, в котором будут храниться имена.
    Storage names_;
};
