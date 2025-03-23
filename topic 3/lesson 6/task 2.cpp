#include <iostream>
#include <string>

using namespace std;

int main() 
{
    int a = 0, b = 0;       // Исходные данные

    // Ввод исходных данных
    cin >> a >> b;

    // Расчет и вывод результата
    if (a == 0)             // При a=0 выводится строка в зависимости от значения b
        {
            string msg = ((b == 0) ? "бесконечное число корней" : "нет корней");
            cout << msg << endl;
        }
    else                    // Иначе вычисляется и выводится корень уравнения
        {
            double x = static_cast<double>(-b) / a;
            cout << x << endl;
        }
}
