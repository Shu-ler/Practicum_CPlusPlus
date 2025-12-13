#pragma once

#include "date.h"
#include <vector>

class BudgetManager {
public:
    static const Date START_DATE;
    static const Date END_DATE;
    static const int DAY_COUNT;

    BudgetManager();

    // Команды
    void Earn(int from, int to, double value);
    void Spend(int from, int to, double value);
    void PayTax(int from, int to, int rate);
    double ComputeIncome(int from, int to) const;

private:
    // Храним два вектора: заработано и потрачено
    std::vector<double> earned_;
    std::vector<double> spent_;
};
