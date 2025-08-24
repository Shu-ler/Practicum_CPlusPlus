#include <cstdint>
#include <iostream>

using namespace std;

// упростите эту экспоненциальную функцию,
// реализовав линейный алгоритм
int64_t T(int i) {
    if (i <= 1) {
        return 0;
    }
    int64_t p0 = 0, p1 = 0, p2 = 1;

    for (int t = 2; t < i; ++t) {
        int64_t n = p0 + p1 + p2;
        p0 = p1;
        p1 = p2;
        p2 = n;
    }

    return p2;
}

int main() {
    int i;

    while (true) {
        cout << "Enter index: "s;
        if (!(cin >> i)) {
            break;
        }

        cout << "Ti = "s << T(i) << endl;
    }
}
