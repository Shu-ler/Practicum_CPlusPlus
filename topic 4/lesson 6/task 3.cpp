#include <iostream>
#include <vector>
#include <string>

#define REVERS_i number_size - i - 1
#define CUR_DIGIT number[i]
#define ALL_ELEMENT (size_t i = 0; i < number_size; ++i)

using namespace std;

int main()
{
    string str_number;       // Строка хранения числа
    vector<int> number;      // Вектор посимвольного хранения числа
    size_t number_size;      // Длина числа
    bool in_the_mind = true; // Триггер "1 в уме"

    // Ввод числа
    cin >> str_number;

    // Сохранение числа в вектор в обратном порядке
    number_size = str_number.size();
    for ALL_ELEMENT
        {
            number.push_back(static_cast<int>(str_number[REVERS_i] - '0'));
        }

    // Прибавление единицы
    for ALL_ELEMENT
        {
            if (CUR_DIGIT == 9)
            {
                CUR_DIGIT = 0;
            }
            else
            {
                ++CUR_DIGIT;
                in_the_mind = false;
                break;
            }
        }

    // Обработка ситуации "Все девятки" - добавление лидирующей единицы
    if (in_the_mind)
    {
        number.push_back(1);
        ++number_size;
    }

    // Вывод элементов вектора
    for ALL_ELEMENT
        {
            cout << number[REVERS_i];
        }
    cout << endl;
}