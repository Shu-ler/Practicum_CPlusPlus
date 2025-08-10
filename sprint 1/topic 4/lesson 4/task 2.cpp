#include <iostream>
#include <cinttypes>

using namespace std;

bool IsCorrectInput(int N)
{

    bool IsCorrectInput = (N > 0) && (N < 63);

    if (!IsCorrectInput)
    {
        cout << "Wrong input";
    }

    return IsCorrectInput;
}

int main()
{
    int N = 0;                    // Вводимое число
    bool first_occurrence = true; // Триггер первого вхождения
    uint64_t agr = 1;             // Значение степени

    // Ввод исходных
    cin >> N;

    // Расчёт и вывод степеней в строку
    if (IsCorrectInput(N))
    {
        for (int i = 0; i < N; ++i)
        {
            agr *= 2;
            if (!first_occurrence)
            {
                cout << ", ";
            }
            cout << agr;
            first_occurrence = false;
        }
    }

    // Вывод конца строки
    cout << endl;
}
