#include <string>
#include <iostream>

using namespace std;

int main() 
{
    
    // Декларация переменных
    string haystack, needle, replacement;

    // Ввод строк
    getline(cin, haystack);                // Исходная строка
    getline(cin, needle);                  // Сигнатура замены
    getline(cin, replacement);             // Строка замены

    // Поиск и замена
    size_t needle_index = haystack.find(needle);
    if (needle_index != string::npos)
        {
            size_t needle_size = needle.size();

            haystack = haystack.substr(0, needle_index) 
                        + replacement
                        + haystack.substr(needle_index + needle_size);
        }
   
    // Вывод результата
    cout << haystack << endl;
}
