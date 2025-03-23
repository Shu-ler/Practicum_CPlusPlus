/* Напишите программу, которая будет считывать из потока ввода целое число N. 
Если число от 1 до 10 включительно, то программа выведет его римскими цифрами, 
в противном случае выведет сообщение Введено неожиданное число в поток ошибок.
В качестве символов необходимо использовать заглавные латинские буквы: X, V, I. 
*/
#include <iostream>
#include <string>

using namespace std;

int main()
{
    
    int N = 0;                                      // Буфер ввода
    string msg = "Введено неожиданное число";       // Буфер вывода. Инициализирован сообщением об ошибке
    bool input_ok = true;                           // Флаг корректного ввода

    // Ввод числа
    cin >> N;           

    // Разбор ввода
    if (N == 1) {msg = "I";}
    else if (N == 2) {msg = "II";}
    else if (N == 3) {msg = "III";}
    else if (N == 4) {msg = "IV";}
    else if (N == 5) {msg = "V";}
    else if (N == 6) {msg = "VI";}
    else if (N == 7) {msg = "VII";}
    else if (N == 8) {msg = "VIII";}
    else if (N == 9) {msg = "IX";}
    else if (N == 10) {msg = "X";}
    else {input_ok = false;}

    // Вывод результата
    if (input_ok) {cout << msg << endl;}
    else {cerr << msg << endl;}

}
