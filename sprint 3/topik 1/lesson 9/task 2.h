#pragma once

#include <cstdlib>

class Rational {
public:
    Rational(int numerator, int denominator)
    : numerator_(numerator)
    , denominator_(denominator) {
      if (denominator_ == 0) {
          std::abort();
      }
  }

explicit operator double() const {
    return  numerator_ * 1.0 / denominator_;
}

private:
    int numerator_ = 0;
    int denominator_ = 1;
};
