#include <iostream>
#include <string>

using namespace std;

int main() {

    string line;            // Буфер ввода строки
    int line_size = 0;      // Длина строки
    int line_count = 0;     // Количество введенных строк
    int msg_size = 0;       // Общая длина всего текста

    // Ввод строк в цикле
    do {
        getline(cin, line);
        line_size = line.size();
        msg_size += line_size;
        line_count = (line_size > 0) ? line_count + 1 : line_count;
    } while (line_size > 0);

    // Вывод количества строк и суммарной длины в одну строку
    cout << line_count << " " << msg_size << endl;
}
