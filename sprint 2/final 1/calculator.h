#pragma once

#include <string>

using Number = double;

class Calculator {
public:
    void Set(Number n);
    Number GetNumber() const;
    void Add(Number n);
    void Sub(Number n);
    void Div(Number n);
    void Mul(Number n);
    void Pow(Number n);
    void Save();
    void Load();
    bool HasMem() const;
    std::string GetNumberRepr() const;
private:
    Number num_in_calc_ = 0;        // Число в калькуляторе
    Number calc_memory_ = 0;        // Число в ячейке памяти
    bool num_in_memory_ = false;    // Тоиггер занесения числа в ячейку памяти
};
