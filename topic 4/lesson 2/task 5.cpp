#include <iostream>
#include <string>
#include <cctype>

#define Letter  str[str_size]

using namespace std;

int main() {

    string type;            // Тип регистра: upper или lower.
    string str;             // Буфер ввода
    bool Upper = false;     // Признак перевода в верхний регистр
    int str_size;           // Длина строки, она же будет бегунком по строке

    
    // Ввод 
    getline(std::cin, type);
    getline(std::cin, str);

    // Анализ преобразования
    Upper = (type == "upper");

    // Преобразование строки
    str_size = str.size();
    while (str_size >= 0) {
        --str_size;
        if (isalpha(Letter)) {
            Letter = Upper ? toupper(Letter) : tolower(Letter);
        }
    }

    // Вывод измененной строки
    cout << str << endl;
}
