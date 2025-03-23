#include <iostream>
#include <string>

using namespace std;

// Проверка на коррекный набор символов
bool is_correct_symbols(string address, string correct_symbols)
{
    return (address.find_first_not_of(correct_symbols) == string::npos);
}

// Возвращает позицию единственного коммерческого Эт. 
// Если нет @ или больше одного - возвращает ноль.
size_t position_of_the_only_one_at(string address)
{
    const string at = "@";                      // Сивол коммерческого Эт
    size_t position_at = 0;                     // Результат проверки адреса

    size_t first_at = address.find(at);

    if ((first_at != string::npos) 
        & (first_at == address.rfind(at)))
        {
            position_at = first_at;            
        }

    return position_at;
}

bool is_correct_dots_after_at(string addr_part)
{
    const string dot = ".";                         // Сивол коммерческого Эт

    bool is_correct = true;
    size_t first_dot = addr_part.find(dot);         // Позиция первой точки
    size_t last_dot = addr_part.rfind(dot);         // Позиция последней точки
    size_t addr_part_size = addr_part.size();       // Длина части адреса

    if ((first_dot == string::npos)
        || (first_dot == 0)
        || (last_dot == addr_part_size - 1))
        {
            is_correct = false;
        }

    return is_correct;
}

int main()
{
    const string correct_symbols =              // Набор допустимых символов адреса email
         "abcdefghijklmnopqrstuvwxyz"
         "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
         "0123456789._-+@";

    string msg = "Некорректный email";          // Результирующее сообщение, инициализированное ошибкой
    string line;                                // Буфер ввода
    size_t position_at;                         // Позиция коммерческого Эт

    // Ввод адреса
    getline(cin, line);

    // Анализ адреса
    if ((is_correct_symbols(line, correct_symbols))
        && (position_at = position_of_the_only_one_at(line))
        && (is_correct_dots_after_at(line.substr(position_at + 1)))) 
        {
            // При корректном адресе выводим имя ящика и домен через перевод строки
            msg = line.substr(0, position_at) + "\n" + line.substr(position_at + 1);
        }

    // Вывод результата
    cout << msg << endl;

}
