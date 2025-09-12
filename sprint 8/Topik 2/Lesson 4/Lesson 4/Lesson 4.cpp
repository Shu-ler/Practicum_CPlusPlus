﻿#include <cassert>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>
#include <utility>
#include <deque>

using namespace std;

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
    vector<typename RandomIt::value_type> pool(first, last);
    size_t cur_pos = 0;
    while (!pool.empty()) {
        *(first++) = std::move(pool[cur_pos]);
        pool.erase(pool.begin() + cur_pos);
        if (pool.empty()) {
            break;
        }
        cur_pos = (cur_pos + step_size - 1) % pool.size();
    }
}

vector<int> MakeTestVector() {
    vector<int> numbers(10);
    iota(begin(numbers), end(numbers), 0);
    return numbers;
}

void TestIntVector() {
    const vector<int> numbers = MakeTestVector();
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
        assert(numbers_copy == numbers);
    }
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
        assert(numbers_copy == vector<int>({ 0, 3, 6, 9, 4, 8, 5, 2, 7, 1 }));
    }
}

// Это специальный тип, который поможет вам убедиться, что ваша реализация
// функции MakeJosephusPermutation не выполняет копирование объектов.
// То, как работает этот класс, мы расскажем далее в нашем курсе

struct CopyTrackingInt {
    int value;

    CopyTrackingInt(int value) noexcept
        : value(value) {
    }

    CopyTrackingInt(const CopyTrackingInt& other) noexcept
        : value(other.value) {
        ++kAmountOfCopies;
    }

    CopyTrackingInt& operator=(const CopyTrackingInt& other) noexcept {
        ++kAmountOfCopies;

        if (&other == this) {
            return *this;
        }

        value = other.value;

        return *this;
    }

    CopyTrackingInt(CopyTrackingInt&&) = default;
    CopyTrackingInt& operator=(CopyTrackingInt&&) = default;

    inline static size_t kAmountOfCopies = 0;
};

bool operator==(const CopyTrackingInt& lhs, const CopyTrackingInt& rhs) {
    return lhs.value == rhs.value;
}

ostream& operator<<(ostream& os, const CopyTrackingInt& v) {
    return os << v.value;
}

void TestAvoidsCopying() {
    vector<CopyTrackingInt> numbers;
    numbers.push_back({ 1 });
    numbers.push_back({ 2 });
    numbers.push_back({ 3 });
    numbers.push_back({ 4 });
    numbers.push_back({ 5 });

    MakeJosephusPermutation(begin(numbers), end(numbers), 2);

    vector<CopyTrackingInt> expected;
    expected.push_back({ 1 });
    expected.push_back({ 3 });
    expected.push_back({ 5 });
    expected.push_back({ 4 });
    expected.push_back({ 2 });

    assert(numbers == expected);
    assert(CopyTrackingInt::kAmountOfCopies == 0);
}

int main() {
    TestIntVector();
    TestAvoidsCopying();
    return 0;
}
