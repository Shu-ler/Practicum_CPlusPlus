#include <iostream>
#include <vector>

using namespace std;

int main()
{
    int N = 0;          // Длина вектора
    vector<int> series; // Вектор с числами
    int num;            // Буфер ввода чисел в серию
    int unique = 0;     // Счетчик уникальных элементов вектора
    bool is_unique;     // Признак уникальности

    // Ввод количества чисел и самих чисел
    cin >> N;
    for (size_t i = 0; i < N; i++)
    {
        cin >> num;
        series.push_back(num);
    }

    // Подсчёт уникальных
    for (size_t i = 0; i < N; i++)
    {
        is_unique = true;
        for (size_t j = 0; j < N; j++)
        {
            if ((i != j) && (series[i] == series[j]))
            {
                is_unique = false;
                break;
            }
        }
        unique = unique + is_unique;
    }

    // Вывод результата
    cout << unique << endl;
}