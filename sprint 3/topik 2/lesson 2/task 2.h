#pragma once
#include <iostream>

struct PointInt {
    PointInt() = default;
    PointInt(int x0, int y0)
       : x(x0), y(y0) {}

    int x = 0;
    int y = 0;
};

struct PointDouble {
    PointDouble() = default;
    PointDouble(double x0, double y0)
       : x(x0), y(y0) {}

    double x = 0;
    double y = 0;
};

class Vector2DInt { 
public:
    Vector2DInt(PointInt p_start, PointInt p_end)
      : start_(p_start)
      , end_(p_end)
    {
    }
    
    int GetLengthSq() const {
        int dx = start_.x - end_.x;
        int dy = start_.y - end_.y;
        return dx * dx + dy * dy;
    }
    
    // Проверяет пересечение оси.
    // Напишите этот метод.
    bool CheckAxisIntersection();
private:
    bool HaveSameSign(int a, int b) {
        return (a > 0 && b > 0) || (a < 0 && b < 0);
}
    PointInt start_;
    PointInt end_;
};

class Vector2DDouble { 
public:
    Vector2DDouble(PointDouble p_start, PointDouble p_end)
      : start_(p_start)
      , end_(p_end)
    {
    }
    
    double GetLengthSq() const {
        double dx = start_.x - end_.x;
        double dy = start_.y - end_.y;
        return dx * dx + dy * dy;
    }
    
    // Проверяет пересечение оси.
    // Напишите этот метод.
    bool CheckAxisIntersection();
    
private:
    bool HaveSameSign(double a, double b) {
        return (a > 0 && b > 0) || (a < 0 && b < 0);
    }

    PointDouble start_;
    PointDouble end_;
};

inline bool Vector2DInt::CheckAxisIntersection() {
    return !HaveSameSign(start_.x, end_.x) || !HaveSameSign(start_.y, end_.y);
}

inline bool Vector2DDouble::CheckAxisIntersection() {
    return !HaveSameSign(start_.x, end_.x) || !HaveSameSign(start_.y, end_.y);
}
