#include <iostream>
#include <string>
#include <cmath>
#include "calculator.h"

// Команды калькулятора
const std::string cmd_plus = "+";
const std::string cmd_minus = "-";
const std::string cmd_multiply = "*";
const std::string cmd_divide = "/";
const std::string cmd_degree = "**";
const std::string cmd_set = ":";
const std::string cmd_equally = "=";
const std::string cmd_clear = "c";
const std::string cmd_store = "s";
const std::string cmd_read = "l";
const std::string cmd_quit = "q";

// Сообщения об ошибках
const std::string error_num = "Error: Numeric operand expected";
const std::string error_op = "Error: Unknown token ";
const std::string error_mem = "Error: Memory is empty";

bool NeedSecondNumber(std::string command) {
    return (command == cmd_plus) 
        || (command == cmd_minus) 
        || (command == cmd_multiply) 
        || (command == cmd_divide) 
        || (command == cmd_set) 
        || (command == cmd_degree);
}

bool ReadNumberIfNecessary(std::string command, Number &next_number, bool next_number_ok) {
    bool result = true;
    next_number_ok = false;
    if (NeedSecondNumber(command)) {
        result = next_number_ok = ReadNumber(next_number);
    }
    return result;
}

bool ReadNumber(Number &numbuffer) {
    bool number_input_ok = true;
    if (!(std::cin >> numbuffer)) {
        std::cerr << error_num << std::endl;
        number_input_ok = false;
    }
    return number_input_ok;
}

bool RunCalculatorCycle() {
    Number num_in_calc;  // Число в калькуляторе
    Number next_number;  // Следующее число
    Number calc_memory;  // Число в ячейке памяти
    std::string command; // Обрабатываемая команда

    bool continue_calc;              // Триггер продолжения вычислений
    bool num_in_memory = false;      // Тоиггер занесения числа в ячейку памяти
    bool completed_normally = false; // Результат завершения функции RunCalculatorCycle
    bool next_number_ok = true;      // Флаг корректного ввода числа

    // Ввод первого числа, при успехе - продолжение работы калькулятора
    continue_calc = ReadNumber(num_in_calc);

    // Цикл ввода команд и чисел
    while (continue_calc) {
        std::cin >> command;
        next_number_ok = true;
        continue_calc = ReadNumberIfNecessary(command, next_number, next_number_ok);

        if (command == cmd_plus && next_number_ok) {
            num_in_calc += next_number;
        }
        else if (command == cmd_minus && next_number_ok) {
            num_in_calc -= next_number;
        }
        else if (command == cmd_multiply && next_number_ok) {
            num_in_calc *= next_number;
        }
        else if (command == cmd_divide && next_number_ok) {
            num_in_calc /= next_number;
        }
        else if (command == cmd_degree && next_number_ok) {
            num_in_calc = pow(num_in_calc, next_number);
        }
        else if (command == cmd_set && next_number_ok) {
            num_in_calc = next_number;
        }
        else if (command == cmd_store) {
            calc_memory = num_in_calc;
            num_in_memory = true;
        }
        else if (command == cmd_read) {
            if (num_in_memory) {
                num_in_calc = calc_memory;
            }
            else {
                std::cerr << error_mem << std::endl;
                continue_calc = false;
            }
        }
        else if (command == cmd_equally) {
            std::cout << num_in_calc << std::endl;
        }
        else if (command == cmd_clear) {
            num_in_calc = 0;
        }
        else if (command == cmd_quit) {
            continue_calc = false;
            completed_normally = true; // Выход из цикла штатный
        }
        else {
            if (next_number_ok) {
                std::cerr << error_op << command << std::endl;
            }
            continue_calc = false;
        }
    }
    return completed_normally;
}
