#pragma once

#include <array>
#include <stdexcept>

template <typename T, size_t N>
class StackVector {
public:
    explicit StackVector(size_t a_size = 0) : size_{ a_size } {
        if (a_size > N) {
            using namespace std::literals;
            throw std::invalid_argument("Size exceeds capacity"s);
        }
    }

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    auto begin() { 
        return data_.begin();
    };

    auto end() {
        return data_.data() + size_;
    };

    auto begin() const {
        return data_.begin();
    };

    auto end() const {
        return data_.data() + size_;
    };

    size_t Size() const {
        return size_;
    };

    size_t Capacity() const {
        return N;
    };

    void PushBack(const T& value) {
        if (size_ >= N) {
            using namespace std::literals;
            throw std::overflow_error("The vector is full"s);
        }
        else {
            data_[size_++] = value;
        }
    };

    T PopBack() {
        if (size_ == 0) {
            using namespace std::literals;
            throw std::underflow_error("The vector is empty"s);
        }
        else {
            return data_[--size_];
        }
    };

private:
    std::array<T, N> data_{};
    size_t size_ = 0;
};
