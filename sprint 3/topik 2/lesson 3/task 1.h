#pragma once

template <typename T>
struct Point {
    T x;
    T y;
};

template<typename T>
class Vector2D { 
public:
    Vector2D(Point<T> p_start, Point<T> p_end)
      : start_(p_start)
      , end_(p_end)
    {
    }

    // Считает квадрат длины.
    T GetLengthSq() const {
        T dx = start_.x - end_.x;
        T dy = start_.y - end_.y;
        return dx * dx + dy * dy;
    };
    
    // Присваивает начало отрезка.
    void SetStart(const Point<T>& p_start) {
        start_ = p_start; 
    };
    
    // Присваивает конец отрезка.
    void SetEnd(const Point<T>& p_end) {
        end_ = p_end;
    };
    
    // Возвращает начало отрезка.
    Point<T> GetStart() const {
        return start_;
    };
    
    // Возвращает конец отрезка.
    Point<T> GetEnd() const {
        return end_;
    };

private:
    Point<T> start_; // Начало отрезка.
    Point<T> end_; // Конец отрезка.
};
