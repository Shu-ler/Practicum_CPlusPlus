#pragma once

#include "date.h"
#include <vector>

class BudgetManager {
public:
    static const Date START_DATE;
    static const Date END_DATE;
    static const int DAY_COUNT;

    BudgetManager();

    // Обработать запрос
    void Earn(int from, int to, double value);
    void PayTax(int from, int to);
    double ComputeIncome(int from, int to) const;

private:
    // Индекс дня = количество дней от START_DATE
    std::vector<double> earnings_;  // доходы по дням
};
