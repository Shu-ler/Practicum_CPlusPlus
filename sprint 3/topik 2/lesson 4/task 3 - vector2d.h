#include <iostream>
#pragma once

template <typename T>
struct Point {
    T x, y;
};

template<typename T>
class Vector2D { 
public:
    Vector2D() = default;
    Vector2D(Point<T> p_start, Point<T> p_end) 
        : start_(p_start), end_(p_end) { } 

    template<typename S>
    friend std::ostream& operator<<(std::ostream& os, const Vector2D<S>& vec);
    template<typename S>
    friend std::istream& operator>>(std::istream& is, Vector2D<S>& vec);

private:
    Point<T> start_;
    Point<T> end_; 
};

// Вывод Point.
template <typename T>
std::ostream& operator<< (std::ostream &os, const Point<T> &point) {
    using namespace std::literals;
    return os << "("s << point.x << "; "s << point.y << ")"s;
};

// Ввод Point.
template<typename T>
std::istream& operator>>(std::istream& is, Point<T>& point) {
    return is >> point.x >> point.y;    
}

// Вывод Vector2D.
template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector2D<T>& vec) {
    using namespace std::literals;
    return os << "["s << vec.start_ << ", "s << vec.end_ << "]"s;
}

// Ввод Vector2D.
template<typename T>
std::istream& operator>>(std::istream& is, Vector2D<T>& vec) {
    return is >> vec.start_ >> vec.end_;
}
