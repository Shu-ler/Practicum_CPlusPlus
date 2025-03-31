#include <iostream>

using namespace std;

int main()
{

    int N;                    // Количество вводимых чисел
    int Num;                  // Вводимое число
    bool ThisFirstNum = true; // Триггер первого числа
    int cur_max = 0;          // Максимальное числа
    int cur_next = 0;         // Второе из максимальных чисел

    // Ввод количества чисел
    cin >> N;

    // Ввод чисел и поиск min и max
    for (int i = 0; i < N; ++i)
    {
        cin >> Num;

        // Обработка ввода первого числа
        if (ThisFirstNum)
        {
            ThisFirstNum = false; // Сброс триггера первого ввода
            cur_max = Num;        // Инициализация
            cur_next = Num;       // Обоих значений первым введенным числом
        }

        // Обработка ввода второго числа
        else if ((cur_max == cur_next))
        {
            if (Num >= cur_max)
            {
                cur_max = Num;
            }
            else
            {
                cur_next = Num;
            }
        }

        // Обработка получения самого большого числа
        else if (Num >= cur_max)
        {
            cur_next = cur_max;
            cur_max = Num;
        }

        // Обработка получения самого большого числа
        else if (Num >= cur_next)
        {
            cur_next = Num;
        }
    }

    // Упорядоченный вывод двух самых больших чисел
    cout << cur_max << " "s << cur_next << endl;
}
