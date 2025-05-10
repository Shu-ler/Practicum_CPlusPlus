#include <iostream>
#include <utility>

// Определим пару.
using Vector2I = std::pair<int, int>;

Vector2I operator+(const Vector2I& vect1, const Vector2I& vect2) {
    return Vector2I(vect1.first + vect2.first, vect1.second + vect2.second);
};

Vector2I operator-(const Vector2I& vect1, const Vector2I& vect2) {
    return Vector2I(vect1.first - vect2.first, vect1.second - vect2.second);
};

Vector2I operator*(const Vector2I& vect, int value) {
    return Vector2I(vect.first * value, vect.second * value);
};

void Print(std::ostream& str, const Vector2I& vect){
    str << '{';
    str << vect.first;
    str << ',';
    str << vect.second;
    str << '}';
};

Vector2I Read(std::istream& str) {
    Vector2I res;
    char buf;

    str >> buf >> res.first;
    str >> buf >> res.second;
    str >> buf;

    return res;
};


int main() {
    Vector2I point1;
    Vector2I point2;
    char op_sym;
    int val;

    point1 = Read(std::cin);
    std::cin >> op_sym;
    // Выведем элементы пары.
    switch (op_sym) {
        case '+':
            point2 = Read(std::cin);
            // Тут теперь операция сложения пар.
            Print(std::cout, point1 + point2);
            break;
        case '-':
            point2 = Read(std::cin);
            // Тут вычитание.
            Print(std::cout, point1 - point2);
            break;
        case '*':
            std::cin >> val;
            // А тут умножение пары на число.
            Print(std::cout, point1 * val);
            break;
        default:
            std::cout << "Undefined symbol!";
    }
    std::cout << std::endl;
}
