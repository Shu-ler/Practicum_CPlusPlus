#include <iostream>
#include <string>

#define SYMBOL lit[i]

using namespace std;

int main()
{
    string lit;      // Введенный литерал
    size_t lit_size; // Длина введенного литерала

    bool after_backslash = false;  // Триггер прочитанного бэкслеша
    bool after_backslash0 = false; // Триггер '0' после бэкслеша

    // Ввод литерала
    getline(std::cin, lit);
    lit_size = lit.size();

    // Обработка и посимвольный вывод
    for (size_t i = 1; i < lit_size - 1; ++i)
    {
        if (after_backslash)
        { // Обработка символа, следующего за бэкслешем
            switch (SYMBOL) // Действие в зависимости от символа
            {
            case 'n': // 'n' - вывод перевода строки
                cout << endl;
                break;
            case '0': // '0' - установка триггера окончания вывода
                after_backslash0 = true;
                break;
            default: // Для остальных символов - просто вывод символа
                cout << SYMBOL;
            }
            if (after_backslash0) // Если после бэкслеша '0' - завершение вывода
            {
                break;
            }
            after_backslash = false; // Сброс триггера прочитанного бэкслеша
        }
        else
        { // Обработка бэкслеша
            if (SYMBOL == '\\')
            {
                after_backslash = true;
            }
            else
            {
                cout << SYMBOL;
            }
        }
    }
}
