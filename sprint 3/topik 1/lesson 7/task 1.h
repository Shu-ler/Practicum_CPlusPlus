class Rational
{
public:
    Rational(int numerator, int denominator)
        : numerator_(numerator), denominator_(denominator) {}

    Rational(int numerator)
        : numerator_(numerator) {
        denominator_ = 1;
    }

    // Перегрузка операции сравнения.
    // Вы научитесь этому в следующем уроке.
    bool operator==(const Rational &r) const
    {
        return (numerator_ == r.numerator_) &&
               (denominator_ == r.denominator_);
    }

    Rational &operator=(const int numerator)
    {
        numerator_ = numerator;
        denominator_ = 1;
        return *this;
    }

    Rational &operator=(const Rational&) = default;

private:
    int numerator_ = 0;
    int denominator_ = 1;
};
