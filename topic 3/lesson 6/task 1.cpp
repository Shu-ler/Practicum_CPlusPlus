/*
По определению високосным годом считается тот, чей номер без остатка делится на 4 и при этом не делится на 100.
Помимо этих годов, високосными будут те, номера которых делятся на 400.
Вам предстоит написать программу для определения високосных годов.

Что нужно сделать
Напишите программу, которая считает целое число N — номер года — и определит, является ли год високосным.
Программа должна вывести Да, если год високосный, Нет — если не високосный.
*/
#include <iostream>
#include <string> 

using namespace std;

int main() 
{

    int N = 0;                  // Номер года
    string is_leap = "Нет";     // Високосный ли год

    // Ввод номера года
    cin >> N;

    // Анализ
    if ((N % 400 == 0) ||
        ((N % 100 != 0) && (N % 4 == 0)))
        {is_leap = "Да";}

    // Вывод результирующего сообщения
    cout << is_leap << endl;

}
