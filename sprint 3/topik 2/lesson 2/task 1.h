#pragma once
#include <iostream>

// Хранит координаты точки.
struct PointInt64 {
    PointInt64() = default;
    PointInt64(int64_t x0, int64_t y0) : x(x0), y(y0) {}

    int64_t x = 0;
    int64_t y = 0;
};

// Выводит координаты точки.
// Напишите реализацию в .cpp-файле.
std::ostream &operator<<(std::ostream &os, const PointInt64 &point);

// Хранит отрезок, заданный двумя точками.
class Vector2DInt64 {
public:
    Vector2DInt64(PointInt64 p_start, PointInt64 p_end) {
        this->start_ = p_start;
        this->end_ = p_end;
    }

    int64_t GetLengthSq() const { // Считает квадрат длины.
        int64_t dx = start_.x - end_.x,
            dy = start_.y - end_.y;
        return dx * dx + dy * dy;
    }

private:
    PointInt64 start_;
    PointInt64 end_;
};
