#pragma once

#include <cstdlib> // std::abort.

class ArithmeticProgression {
public:
    ArithmeticProgression(double start, double diff) : 
        start_(start), diff_(diff) {}

        double operator[](int index) {
            if (index < 1) { 
                std::abort; 
            } else { 
                return start_ + diff_ * (index - 1);
            }
        }
private:
    double start_;   // Начальный член.
    double diff_;    // Разность.
};
