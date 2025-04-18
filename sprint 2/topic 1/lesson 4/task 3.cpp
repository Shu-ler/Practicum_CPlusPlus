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
    return FindLength(PointDifference(t.p1, t.p2)) 
        + FindLength(PointDifference(t.p2, t.p3)) 
        + FindLength(PointDifference(t.p1, t.p3));
}

int main()
{
    Triangle t = ReadTriangle();
    std::cout << FindPerimeter(t) << std::endl;
}
