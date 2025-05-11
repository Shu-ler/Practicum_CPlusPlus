#pragma once

#include <iostream>

class Rational {
public:
    Rational() = default;
    Rational(int numerator, int denominator)
     : numerator_{numerator}, denominator_{denominator} {}
    
    friend std::ostream& operator<<(std::ostream& os, const Rational& rat);
    friend std::istream& operator>>(std::istream& is, Rational& rat);

private:
    int numerator_ = 0;
    int denominator_ = 1;
};

inline std::ostream& operator<<(std::ostream& os, const Rational& rat) { 
    if (rat.denominator_ == 1) {
        os << rat.numerator_;
    } else {
        os << rat.numerator_ << " / " << rat.denominator_;
    }
    return os;
}

inline std::istream& operator>>(std::istream& is, Rational& rat) { 
    char div;   // Буфер ввода разделителя
    int n = 0;  // Буферы ввода числителя
    int d = 0;  // Буферы ввода знаменателя

    if (!(is >> n)) {
        return is;   
    }
    
    // Ввод разделителя с удалением возможного лидирующего пробела
    if (!(is >> std::ws >> div)) {
        rat.numerator_ = n;
        rat.denominator_ = 1;
        is.clear();
        return is;
    }

    // Проверка знака разделителя
    if (div != '/') {
        rat.numerator_ = n;
        rat.denominator_ = 1;
        is.unget();
        is.clear();
        return is;
    }

    // Проверка ввода знаменателя
    if (!(is >> d) || (d == 0)) {
        is.setstate(std::ios::failbit);
    } else {
        rat.numerator_ = n;
        rat.denominator_ = d;
    }
    return is;
}
