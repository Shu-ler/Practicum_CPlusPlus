#include <iostream>

using namespace std;

// Это декларация функции IsPrime.
// Она позволит использовать
// функцию внутри вашей программы.
// Не нужно писать реализацию этой функции.
bool IsPrime(int);

int main() 
{
    int i = 1;

    while (i <= 100)
        {
            if (IsPrime(i)) {cout << i << endl;}
            i++;
        }
}