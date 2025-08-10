#include <iostream>
#include <string>
#include <cctype>

#define SYMBOL line[i]

using namespace std;

int main()
{
    string line;          // Буффер для ввода строки
    size_t line_size;     // Длина введенной строки
    bool at_alfa = false; // Триггер предыдущего символа

    // Ввод строки
    getline(std::cin, line);
    line_size = line.size();

    // Посимвольное чтение и вывод
    for (size_t i = 0; i < line_size; ++i)
    {
        cout << SYMBOL;
        if (isdigit(SYMBOL))
        {
            if (at_alfa)
            {
                cout << endl;
                break;
            }
            at_alfa = false;
        }
        at_alfa = isalpha(SYMBOL);
    }
}
