#pragma once
#include <utility>

// Реализуйте эту функцию.
inline std::pair<int, int> FibonacciRec(int n) {
    if (n < 2) {
        return std::make_pair(1, 0);
    }
    auto [fn, fn_min_1] = FibonacciRec(n - 1);
    return std::make_pair(fn + fn_min_1, fn);
}

inline int Fibonacci(int n) {
    if (n == 0) {
        return 0;
    }
    return FibonacciRec(n).first;
}
