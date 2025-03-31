#include <iostream>

using namespace std;

int main() {
    
    int N;          // Количество чисел
    int element;    // Буфер ввода
    int summ = 0;   // Сумма чисел
    double average; // Среднее значение

    // Ввод количества элементов
    cin >> N;

    // Ввод элементов в цикле
    for (int i = 0; i < N; ++i) {
        cin >> element;
        summ += element;
    }

    // Вычисление и вывод среднего
    average = static_cast<double>(summ) / N;
    cout << average << endl;

}
