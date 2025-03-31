#include <iostream>
#include <string>
#include <cctype>

using namespace std;

int main()
{

    int num_count = 0;      // Счетчик количества чисел
    bool at_number = false; // Триггер прохода по числу
    string line;            // Буффер для ввода строки
    size_t line_size;       // Длина введенной строки

    // Ввод строки
    getline(std::cin, line);

    // Анализ
    line_size = line.size();
    for (size_t i = 0; i < line_size; ++i)
    {
        if (isdigit(line[i]))
        {
            if (!at_number)
            {
                at_number = true;
                ++num_count;
            }
        }
        else
        {
            at_number = false;
        }
    }

    // Вывод результата
    cout << num_count << endl;
}
