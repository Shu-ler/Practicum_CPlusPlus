#include <cstdint>
#include <iostream>
#include <limits>

using namespace std;

int main() {
    int64_t a;
    int64_t b;

    // Считывание двух слагаемых
    cin >> a >> b;

    // Проверка переполнения и вывод
    if ((a > 0 && b > 0 && a > numeric_limits<std::int64_t>::max() - b) 
        || (a < 0 && b < 0 && a < numeric_limits<std::int64_t>::min() - b)) {
        cout << "Overflow!"s << endl;
    }
    else {
        cout << a + b << endl;
    }
}
