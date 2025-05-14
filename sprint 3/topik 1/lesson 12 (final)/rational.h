#pragma once
#include <cstdint>
#include <cstdlib>
#include <compare>
#include <numeric>
#include <iostream>

class Rational
{
public:
    Rational() = default;

    Rational(int numerator)
        : numerator_(numerator) {}

    Rational(int numerator, int denominator)
        : numerator_(numerator), denominator_(denominator)
    {
        Reduction(); // Приведение дроби к правильному формату.
    }

    Rational(const Rational& original) {
        numerator_ = original.numerator_;
        denominator_ = original.denominator_;
    }

    Rational operator+(const Rational& r2) const {
        Rational res{numerator_ * r2.denominator_ + denominator_ * r2.numerator_,
            denominator_ * r2.denominator_};
            res.Reduction();
        return res;
    }

    Rational operator-(const Rational& r2) const {
        Rational res{numerator_ * r2.denominator_ - denominator_ * r2.numerator_,
            denominator_ * r2.denominator_};
            res.Reduction();
        return res;
    }

    Rational operator*(const Rational& r2) const {
        Rational res{numerator_ * r2.numerator_, denominator_ * r2.denominator_};
        res.Reduction();
        return res;
    }

    Rational operator/(const Rational& r2) const {
        Rational res{numerator_ * r2.denominator_, denominator_ * r2.numerator_};
        res.Reduction();
        return res;
    }

    Rational& operator+=(const Rational& r2) {
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

    Rational &operator=(const Rational&) = default;
    Rational &operator=(const int numerator)
    {
        numerator_ = numerator;
        denominator_ = 1;
        return *this;
    }

    Rational operator+() const {
        return *this;
    };

    Rational operator-() const {
        auto res = *this;
        res.numerator_ *= -1;
        return res;
    };

    friend std::ostream& operator<<(std::ostream& os, const Rational& rat);
    friend std::istream& operator>>(std::istream& is, Rational& rat);

    Rational Inv() const; 
    int GetNumerator() const;
    int GetDenominator() const;

private:
    void Reduction()
    {
        if (denominator_ == 0)
        {
            std::abort();
        }

        if (denominator_ < 0)
        {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        const int divisor = std::gcd(numerator_, denominator_);
        numerator_ /= divisor;
        denominator_ /= divisor;
    }

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

inline auto operator<=>(const Rational& lhs, const Rational& rhs) {
    std::int64_t ls = static_cast<std::int64_t>(lhs.GetNumerator()) * rhs.GetDenominator();
    std::int64_t rs = static_cast<std::int64_t>(rhs.GetNumerator()) * lhs.GetDenominator();
    return ls <=> rs;
} 

inline auto operator==(const Rational& lhs, const Rational& rhs) {
    return lhs.GetNumerator() * rhs.GetDenominator() ==
    lhs.GetDenominator() * rhs.GetNumerator();
} 

inline Rational Rational::Inv() const {
    return Rational{denominator_, numerator_};
};

inline int Rational::GetNumerator() const {
    return numerator_;
}

inline int Rational::GetDenominator() const {
    return denominator_;
}
