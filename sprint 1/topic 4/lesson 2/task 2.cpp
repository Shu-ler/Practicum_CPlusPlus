#include <iostream>

using namespace std;

int main() 
{
    int N;          // Вводимое число
    
    // Ввод исходного
    cin >> N;

    // Обработка и вывод цифр
    do 
    {
        cout << N % 10;
        N = N / 10;
    } while (N > 0); 

    // Завершение вывода
    cout << endl;
}