#include <iostream>
#include <cstdint>

using namespace std;

/// @brief Вычисляет n-е по счёту число Фибоначчи
/// @param n порядковый номер числа 
/// @return значение числа Фибоначчи
uint64_t GetFibonacci(int n)
{
    int first = 0;                               // Буфер первого предыдущего
    int second = 1;                              // Буфер второго предыдущего
    uint64_t result = (n == 0) ? first : second; // Результат, инициализированный для 0 и 1

    for (int i = 1; i < n; ++i)
    {
        result = first + second;
        first = second;
        second = result;
    }

    return result;
}

int main()
{
    int n;
    cin >> n;
    cout << GetFibonacci(n) << endl;
}