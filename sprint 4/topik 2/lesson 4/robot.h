#pragma once

#include <vector>
#include <optional>

struct MaxCell {
    size_t pos;
    int num;
};

class Robot {
public:
    explicit Robot(size_t max_cells) {
    }

    void GoRight(size_t n_pos) {
        // Ваша реализация метода.
    }

    void GoLeft(size_t n_pos) {
        // Ваша реализация метода.
    }

    void SetPos(size_t start) {
        // Ваша реализация метода.
    }

    void PutBalls(int num) {
        // Ваша реализация метода.
    }

    // Нужно возвратить std::nullopt, если вектор пуст.
    std::optional<MaxCell> CalculateMaxValue() const {
        // Ваша реализация метода.
    }

private:
    std::vector<int> line_{};
    std::vector<int>::iterator current_it_{};
};
