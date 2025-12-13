#include "budget_manager.h"
#include "date.h"

using namespace std::literals;

const Date BudgetManager::START_DATE = Date("2000-01-01"s);
const Date BudgetManager::END_DATE = Date("2100-01-01"s);
const int BudgetManager::DAY_COUNT = Date::ComputeDistance(START_DATE, END_DATE) + 1;

BudgetManager::BudgetManager()
    : earnings_(DAY_COUNT, 0.0) {
}

void BudgetManager::Earn(int from, int to, double value) {
    const int days = to - from + 1;
    const double daily = value / days;
    for (int i = from; i <= to; ++i) {
        earnings_[i] += daily;
    }
}

void BudgetManager::PayTax(int from, int to) {
    for (int i = from; i <= to; ++i) {
        earnings_[i] *= 0.87;  // 100% - 13% = 87%
    }
}

double BudgetManager::ComputeIncome(int from, int to) const {
    double total = 0.0;
    for (int i = from; i <= to; ++i) {
        total += earnings_[i];
    }
    return total;
}
