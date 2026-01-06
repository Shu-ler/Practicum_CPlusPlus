#include <algorithm>
#include <cassert>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>

using namespace std;

template <typename It, typename SumOut, typename SumSqOut, typename MaxOut>
void ComputeStatistics(It first, It last, SumOut&& sum_out, SumSqOut&& sum_sq_out, MaxOut&& max_out) {
    using T = decay_t<decltype(*first)>;

    bool first_element = true;

    // Инициализируем переменные — но будем использовать ТОЛЬКО если нужно
    T sum{};
    T sum_sq{};
    T max{};

    constexpr bool need_sum = !is_same_v<decay_t<SumOut>, nullopt_t>;
    constexpr bool need_sum_sq = !is_same_v<decay_t<SumSqOut>, nullopt_t>;
    constexpr bool need_max = !is_same_v<decay_t<MaxOut>, nullopt_t>;

    for (auto it = first; it != last; ++it) {
        const T& value = *it;

        if (first_element) {
            if constexpr (need_sum) {
                sum = value;
            }
            if constexpr (need_sum_sq) {
                sum_sq = value * value;
            }
            if constexpr (need_max) {
                max = value;
            }
            first_element = false;
        }
        else {
            if constexpr (need_sum) {
                sum = sum + value;
            }
            if constexpr (need_sum_sq) {
                sum_sq = sum_sq + value * value;
            }
            if constexpr (need_max) {
                if (value > max) {
                    max = value;
                }
            }
        }
    }

    // Записываем результаты, только если диапазон не пуст
    if constexpr (need_sum) {
        if (!first_element) {
            sum_out = sum;
        }
    }

    if constexpr (need_sum_sq) {
        if (!first_element) {
            sum_sq_out = sum_sq;
        }
    }

    if constexpr (need_max) {
        if (!first_element) {
            max_out = max;
        }
    }
    // Если диапазон пуст — ничего не делаем (не меняем выходные параметры)
}

struct OnlySum {
    int value;
};

OnlySum operator+(OnlySum l, OnlySum r) {
    return { l.value + r.value };
}
OnlySum& operator+=(OnlySum& l, OnlySum r) {
    return l = l + r;
}

int main() {
    vector input = { 1, 2, 3, 4, 5, 6 };
    int sq_sum;
    std::optional<int> max;

    // Переданы выходные параметры разных типов - std::nullopt_t, int и std::optional<int>
    ComputeStatistics(input.begin(), input.end(), nullopt, sq_sum, max);

    assert(sq_sum == 91 && max && *max == 6);

    vector<OnlySum> only_sum_vector = { {100}, {-100}, {20} };
    OnlySum sum;

    // Поданы значения поддерживающие только суммирование, но запрошена только сумма
    ComputeStatistics(only_sum_vector.begin(), only_sum_vector.end(), sum, nullopt, nullopt);

    assert(sum.value == 20);
}
