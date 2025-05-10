#include <iostream>
#include <string>
#include <utility>

// Определяем пару.
using IntPair = std::pair<int, int>;

// Складывает покомпонентно две пары.
IntPair Add(const IntPair& pair1, const IntPair& pair2) {
    IntPair res;
    res.first = pair1.first + pair2.first;
    res.second = pair1.second + pair2.second;
    
    return res;
};

// Вычитает покомпонентно две пары.
IntPair Sub(const IntPair& pair1, const IntPair& pair2) {
    IntPair res;
    res.first = pair1.first - pair2.first;
    res.second = pair1.second - pair2.second;
    
    return res;
};

// Умножает компоненты пары на число.
IntPair Mul(const IntPair& pair, int value) {
    IntPair res;
    res.first = pair.first * value;
    res.second = pair.second * value;
    
    return res;
};

// Печать и чтение интовой пары.
void Print(std::ostream& out, const IntPair& pair) {
    out << '{';
    out << pair.first;
    out << ',';
    out << pair.second;
    out << '}';
};

IntPair Read(std::istream& inp) {
    IntPair res;
    char buf;

    inp >> buf >> res.first;
    inp >> buf >> res.second;
    inp >> buf;

    return res;
};

int main() {
    std::string str;
    char op_sym;
    IntPair pair1;
    IntPair pair2;
    IntPair res_pair;
    int val;

    pair1 = Read(std::cin);
    std::cin >> op_sym;
    // Выведем элементы пары.
    switch (op_sym) {
        case '+':
            pair2 = Read(std::cin);
            res_pair = Add(pair1, pair2);
            break;
        case '-':
            pair2 = Read(std::cin);
            res_pair = Sub(pair1, pair2);
            break;
        case '*':
            std::cin >> val;
            res_pair = Mul(pair1, val);
            break;
        default:
            std::cout << "Undefined symbol!" << std::endl;
            return 1;
    }
    Print(std::cout, res_pair);
}
