#include <iostream>

using namespace std;

void OddOutput(int Num){
    if (Num % 2) {
        cout << Num << endl;
    }
}

int main() {

    int A, B;           // Исходные данные - границы
    int i = 0;          // Бегунок

    // Ввод границ
    cin >> A >> B;

    // Вывод всех нечётных, кроме последнего
    int step = (A < B) ? 1 : -1;
    for (i = A; i != B; i += step) {
        OddOutput(i);
        }

    // Вывод последнего
    OddOutput(i);

}
