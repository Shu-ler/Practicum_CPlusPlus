#pragma once
#include <cstdint>
#include <compare>

class Rational {
public:
    Rational(int numerator, int denominator)
    : numerator_(numerator)
    , denominator_(denominator) {}
    
    int GetNumerator() const {
        return numerator_;
    }
      
    int GetDenominator() const {
        return denominator_;
    }
    
private:
    int numerator_ = 0;
    int denominator_ = 1;
};

// Напишите здесь перегрузки операций.
inline auto operator<=>(const Rational& lhs, const Rational& rhs) {
    std::int64_t ls = static_cast<std::int64_t>(lhs.GetNumerator()) * rhs.GetDenominator();
    std::int64_t rs = static_cast<std::int64_t>(rhs.GetNumerator()) * lhs.GetDenominator();
    return ls <=> rs;
} 

inline auto operator==(const Rational& lhs, const Rational& rhs) {
    return lhs.GetNumerator() * rhs.GetDenominator() ==
    lhs.GetDenominator() * rhs.GetNumerator();
} 
