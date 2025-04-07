#include <iostream>
#include <string>
#include <cmath>

using namespace std;

int main()
{
    // Команды калькулятора
    const char command_plus = '+';
    const char command_minus = '-';
    const char command_multiply = '*';
    const char command_divide = '/';
    const char command_equally = '=';

    // Переменные
    double number = 0;         // Число в калькуляторе
    double number_add;         // Вводимое число
    double monom = 0;          // Аккумулятор монома
    char command;              // Обрабатываемая команда
    bool continue_calc = true; // Триггер продолжения вычислений

    // Ввод первого числа - сразу в моном. Остальные числа вводятся в number_add
    cin >> monom;

    // Цикл ввода команд и чисел
    while (continue_calc)
    {
        // Ввод команды
        cin >> command;

        // При вводе '=' - сброс триггера продолд=жения вычислений
        continue_calc = (command != command_equally);
        if (continue_calc)
        {
            cin >> number_add; // Ввод числа
            switch (command)
            {
            case command_plus: // Обработка плюса
                number += monom;
                monom = number_add;
                break;

            case command_minus: // Обработка минуса
                number += monom;
                monom = -number_add;
                break;

            case command_multiply: // Обработка умножения
                monom *= number_add;
                break;

            case command_divide: // Обработка деления
                monom /= number_add;
                break;
            }
        }
    }
    // Сброс последнего монома и вывод результата
    number += monom;
    cout << number << endl;
}