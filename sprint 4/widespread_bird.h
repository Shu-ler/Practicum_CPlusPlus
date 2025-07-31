#pragma once
#include <map>
#include <vector>
#include <string>

inline std::string FindWidespreadBird(std::vector<std::string> bird_types) {
    std::string res{};
    int max_cnt = 0;
    std::map<std::string, int> birds_map;

    // Заполнение словаря
    for (auto cur_bird : bird_types) {
        auto [iterator, inserted] = birds_map.insert({cur_bird, 1});
        if (!inserted) {
            birds_map[cur_bird] += 1;
        }
    }

    // Поиск самого популярного птица
    for (auto bird : birds_map) {
        if (bird.second > max_cnt) {
            res = bird.first;
            max_cnt = bird.second;
        }
    }
    return res;
}

inline std::string FindWidespreadBirdAutor(const std::vector<std::string>& types) {
    std::map<std::string, int> bird_count;

    for (const std::string& type : types) {
        bird_count[type]++;
    }

    int current_max = 0;
    std::string widespread_bird;
    for (const auto& [bird, count] : bird_count) {
        if (count > current_max) {
            current_max = count;
            widespread_bird = bird;
        }
    }
    return widespread_bird;
}
