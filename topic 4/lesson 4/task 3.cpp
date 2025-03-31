#include <iostream>

using namespace std;

int main() {
    
    int N;                      // Количество вводимых чисел
    int Num;                    // Вводимое число
    bool ThisFirstNum = true;   // Триггер первого числа
    int max = 0, min = 0;       // Минимальное и максимальное числа

    // Ввод количества чисел
    cin >> N;

    // Ввод чисел и поиск min и max
    for (int i = 0; i < N; ++i){
        cin >> Num;
        if (ThisFirstNum) {
            ThisFirstNum = false;
            min = Num;
            max = Num;
        }
        else {
            if (Num > max) {max = Num;}
            else if (Num < min) {min = Num;}
        }
    }

    // Вывод min и max
    cout << min << " "s << max << endl;
}
