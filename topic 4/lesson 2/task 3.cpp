#include <iostream>

#define BLANK   " "

using namespace std;

int main() {

    int price = 0;          // Цена товара
    int summ = 0;           // Набранная сумма
    int count = 0;          // Количество купюр
    int bill = 0;           // Номинал купюры

    // Ввод цены товара
    cin >> price;

    // Обработка купюр
    while (summ < price) {
        cin >> bill;            // Ввод номинала
        summ += bill;           // Увеличение отмусоленной суммы
        ++count;                // Увеличение счетчика купюр
    }

    // Вывод всех результатов в одну строку
    cout << count << BLANK << (summ - price) << BLANK << bill << endl;
}
