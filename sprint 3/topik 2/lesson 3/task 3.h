#pragma once

template <typename T>
struct Point {
  T x;
  T y;
};

template <typename T, typename D>
class Circle {
public:
    Circle(Point<T> center, D radius)
        : center_(center), radius_(radius) {}

    D GetDiameter() const {
        return 2 * radius_; }

private:
    Point<T> center_;
    D radius_;
};
