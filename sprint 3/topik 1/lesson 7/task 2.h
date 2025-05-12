#pragma once

// Подключаем для использования функции std::gcd,
// Которая находит наибольший общий делитель в методе Reduction.
#include <numeric>
#include <cstdlib>

class Rational {
public:
    Rational(int numerator, int denominator)
    : numerator_(numerator)
    , denominator_(denominator) {
        if (denominator_ == 0) {
            std::abort();
        }
        // Приводим дробь к правильному формату.
        Reduction();
    }	
  
    // Перегрузка оператора сравнения.
    // Вы научитесь этому в следующем уроке.
    bool operator==(const Rational& r) const = default;
  
    // Напишите здесь перегрузки четырёх операций.
    Rational &operator=(const Rational&) = default;

    Rational &operator+=(const Rational& r2) {
        numerator_ = numerator_ * r2.denominator_ + denominator_ * r2.numerator_;
        denominator_ = denominator_ * r2.denominator_;
        Reduction();
        return *this;
    }

    Rational &operator-=(const Rational& r2) {
        numerator_ = numerator_ * r2.denominator_ - denominator_ * r2.numerator_;
        denominator_ = denominator_ * r2.denominator_;
        Reduction();
        return *this;
    }

    Rational &operator*=(const Rational& r2) {
        numerator_ = numerator_ * r2.numerator_;
        denominator_ = denominator_ * r2.denominator_;
        Reduction();
        return *this;
    }
    
    Rational &operator/=(const Rational& r2) {
        numerator_ = numerator_ * r2.denominator_;
        denominator_ = denominator_ * r2.numerator_;
        Reduction();
        return *this;
    }

private:
    // Метод для приведения дроби к корректной форме.
    void Reduction() {
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        const int divisor = std::gcd(numerator_, denominator_);
        numerator_ /= divisor;
        denominator_ /= divisor;
    }

private:
    int numerator_ = 0;
    int denominator_ = 1;
};
