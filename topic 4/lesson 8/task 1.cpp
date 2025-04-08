#include <iostream>
#include <string>
#include <cmath>

using namespace std;

// Ввод числа типа double
// При успехе возвращает true, при неудаче - сообщает об ошибке и возвращает false
bool NumInput(double &numbuffer)
{
    // Сообщения об ошибках
    const string error_num = "Error: Numeric operand expected";

    // Ввод числа
    bool NumInput;
    NumInput = (!(cin >> numbuffer)) ? false : true;

    // Проверка корректности ввода
    if (!NumInput)
    {
        cerr << error_num << endl;
    }

    return NumInput;
}

int main()
{
    // Сообщения об ошибках
    const string error_op = "Error: Unknown token ";

    // Команды калькулятора
    const char command_plus = '+';
    const char command_quit = 'q';

    // Переменные
    double number;         // Число в калькуляторе
    char command;          // Обрабатываемая команда
    string command_buffer; // Вводимая команда
    double number_add;     // Число в калькуляторе
    bool continue_calc;    // Триггер продолжения вычислений

    // Ввод первого числа, при успехе - продолжение работы калькулятора
    continue_calc = NumInput(number);

    // Цикл ввода команд и чисел
    while (continue_calc)
    {
        cin >> command_buffer;
        if (command_buffer.size() == 1)
        {
            command = command_buffer[0];

            switch (command)
            {
            case command_plus:                          // Обработка плюса - ввод числа, при успехе - вывод суммы
                continue_calc = NumInput(number_add);
                if (continue_calc)
                {
                    number += number_add;
                    cout << number << endl;
                }
                break;

            case command_quit:                          // Обработка выхода из калькулятора
                continue_calc = false;
                break;

            default:                                    // Обработка прочей команды
                cerr << error_op << command << endl;
                continue_calc = false;
                break;
            }
        }
        else                                            // Обработка команд длиннее одного символа
        {
            cerr << error_op << command_buffer << endl;
            continue_calc = false;
        }
    }
}