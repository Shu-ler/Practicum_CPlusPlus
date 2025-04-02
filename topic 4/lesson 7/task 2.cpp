#include <iostream>
#include <cmath>

using namespace std;

int main()
{
    int N = 0; // Размерность таблицы

    // Ввод размерности таблицы
    cin >> N;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << abs(i - j);
        }
        cout << endl;
    }
}
