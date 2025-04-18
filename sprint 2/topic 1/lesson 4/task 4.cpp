#include <iostream>
#include <cmath>

using namespace std::literals;

// Point
struct Point
{
    double x = 0.0;
    double y = 0.0;
};

Point ReadPoint()
{
    Point buffer;
    std::cin >> buffer.x >> buffer.y;
    return buffer;
};

void PrintPoint(Point p)
{
    std::cout << "{" << p.x << ", " << p.y << "}";
}

// Vector2D
struct Vector2D
{
    double x = 0.0;
    double y = 0.0;
};

Vector2D PointDifference(Point p1, Point p2)
{
    Vector2D buf{
        .x = p1.x - p2.x,
        .y = p1.y - p2.y,
    };
    return buf;
};

double FindLength(Vector2D v)
{
    return std::hypot(v.x, v.y);
};

void PrintVector(Vector2D p)
{
    std::cout << "("s << p.x << ", "s << p.y << ")"s;
}

void MovePoint(Point &p, Vector2D v)
{
    p.x += v.x;
    p.y += v.y;
}

// Triangle
struct Triangle
{
    Point p1;
    Point p2;
    Point p3;
};

Triangle ReadTriangle()
{
    Triangle t1;

    t1.p1 = ReadPoint();
    t1.p2 = ReadPoint();
    t1.p3 = ReadPoint();

    return t1;
}

double FindPerimeter(const Triangle &t)
{
    return FindLength(PointDifference(t.p1, t.p2)) + FindLength(PointDifference(t.p2, t.p3)) + FindLength(PointDifference(t.p1, t.p3));
}

void MoveTriangle(Triangle &t, Vector2D v)
{
    MovePoint(t.p1, v);
    MovePoint(t.p2, v);
    MovePoint(t.p3, v);
}

Point GetCenter(const Triangle &t)
{
    Point centr{
        .x = (t.p1.x + t.p2.x + t.p3.x) / 3,
        .y = (t.p1.y + t.p2.y + t.p3.y) / 3,
    };

    return centr;
}

void PrintTriangle(const Triangle &t)
{
    std::cout << "{";
    PrintPoint(t.p1);
    std::cout << ", ";
    PrintPoint(t.p2);
    std::cout << ", ";
    PrintPoint(t.p3);
    std::cout << "}" << std::endl;
}

int main()
{
    Triangle t = ReadTriangle();

    // Исходный центр треугольника.
    Point origin = GetCenter(t);

    // Читаем желаемый центр треугольника.
    Point center = ReadPoint();

    // Находим смещение центра треугольника.
    Vector2D offset = PointDifference(center, origin);

    std::cout << "Исходный центр: ";
    PrintPoint(origin);
    std::cout << std::endl;

    std::cout << "Смещение: ";
    PrintVector(offset);
    std::cout << std::endl;

    MoveTriangle(t, offset);
    PrintTriangle(t);
    std::cout << std::endl;
}
