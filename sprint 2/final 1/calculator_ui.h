#pragma once

#include <iostream>

#include "calculator.h"

class CalculatorUI
{
public:
    // Реализуйте конструктор. Он должен сохранить ссылки
    // на потоки и калькулятор внутрь класса.
    // Используйте для этого список инициализации.
    CalculatorUI(Calculator &calc, std::ostream &out, std::ostream &err);

    // Заготовки следующих двух методов есть в .cpp-файле.
    // Дополните их.
    bool Parse(std::istream &input) const;

private:
    bool ReadNumber(std::istream &input, Number &result) const;

private:
    Calculator &calc_;
    std::ostream &outstream_;
    std::ostream &errstream_;
};
