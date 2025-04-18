#include <iostream>
#include <cmath>

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
};

// Vector2D
struct Vector2D
{
    double x = 0.0;
    double y = 0.0;
};

Vector2D PointDifference(Point p1, Point p2)
{

    Vector2D buf;
    buf.x = p1.x - p2.x;
    buf.y = p1.y - p2.y;

    return buf;
};

double FindLength(Vector2D v)
{
    return std::hypot(v.x, v.y);
};

int main()
{
    Point p1 = ReadPoint();
    Point p2 = ReadPoint();

    // Вычислим расстояние между p1 и p2.
    Vector2D diff = PointDifference(p1, p2);
    std::cout << FindLength(diff) << std::endl;
}
