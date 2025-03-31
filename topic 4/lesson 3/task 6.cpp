#include <iostream>
#include <cmath>

using namespace std;

int main() {
    
    // Исходные данные
    int amount;             // Сумма кредита
    double rate;            // Годовая процентная ставка
    int term;               // Срок в месяцах

    // Параметры для расчета ежемесячного платежа
    double month_rate;      // Месячная процентная ставка
    double ratio;           // Коэффициент аннуитета
    double month_payment;   // Ежемесячный платёж
    double ratio_mid;       // Коэффициент аннуитета - промежуточный для расчета

    // Прочие переменные
    double debt;            // Величина фактического долга
    double debt_part;       // Величина фактического погашения долга
    double percent_part;    // Сумма выплаты процентов
    int month_number;       // Номер месяца

    // Ввод исходных
    cin >> amount >> rate >> term;

    // Вычисление параметров
    month_rate = rate / 12 / 100;
    ratio_mid = pow(1 + month_rate, term);
    ratio = ratio_mid * month_rate / (ratio_mid - 1);
    month_payment = amount * ratio;

    // Ежемесячные платежи
    debt = amount;
    for(month_number = 1; month_number <= term; ++month_number){
        
        // Разбиение ежемесячного платежа
        percent_part = debt * month_rate;
        debt_part = month_payment - percent_part;

        // Уменьшение основного долга
        debt -= debt_part; 

        // Вывод платежей помесячно
        cout << "Месяц: " << month_number;
        cout << " Платёж: " << month_payment;
        cout << " Проценты: " << percent_part;
        cout << " Долг: " << debt_part << endl;
    }
}
