#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    int N = 0; // Количество строк
    int M = 0; // Количество столбцов

    vector<string> cells = {"o "s, "x "s}; // Псевдографика изображения клеток

    // Ввод размерности доски
    cin >> N >> M;

    // Вывод доски
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < M; j++)
        {
            cout << cells[(i + j) % 2];
        }
        cout << endl;
    }
}