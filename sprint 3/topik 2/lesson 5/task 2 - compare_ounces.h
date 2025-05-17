#pragma once

// Структура для хранения обычных унций.
struct Ounce {
    double count;
    inline static double grams = 28.3495; 
};

// Структура для хранения тройских унций.
struct OunceTroy {
    double count;
    inline static double grams = 31.1;
};

template<typename T>
int CompareTwoValues(const T first, const T second) {
    return first < second ? -1 : first > second ? 1 : 0;
}

template<typename T>
int CompareOunces(T value_1, T value_2) {
    return CompareTwoValues(value_1.count, value_2.count);
}

template<typename T, typename U>
int CompareOunces(T value_1, U value_2) {
    return CompareTwoValues(value_1.count * value_1.grams, value_2.count * value_2.grams);
}
