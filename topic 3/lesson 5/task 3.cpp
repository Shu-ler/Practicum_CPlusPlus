/*
Программа должна считать из потока ввода две пары чисел от 1 до 8 включительно — координаты клеток на шахматной доске.
В первой клетке стоит ладья, во второй — другая фигура.
Необходимо определить, сможет ли ладья «съесть» другую шахматную фигуру. Ответ вывести строкой «Да» или «Нет».
*/
#include <iostream>
#include <string>

using namespace std;

// Возвращает "Да", если возможна атака ладьи, и "Нет" в противном случае
string attack_is_possible(int my_chess_x, int my_chess_y, int chess_x, int chess_y)
{
    string attack_is_possible = "Нет";  // Сообщение о возможной атаке

    // Анализ пересечения координат
    if ((my_chess_x == chess_x) || (my_chess_y == chess_y)) {attack_is_possible = "Да";}

    return attack_is_possible;
}

int main() 
{
    int rook_x = 1, rook_y = 1;         // Координаты ладьи
    int chess_x = 1, chess_y = 1;       // Координаты другой фигуры
 
    // Ввод координат фигур
    cin >> rook_x >> rook_y;
    cin >> chess_x >> chess_y;

    // Вывод
    cout << attack_is_possible(rook_x, rook_y, chess_x, chess_y) << endl;
}