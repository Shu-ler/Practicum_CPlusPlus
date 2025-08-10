#include <iostream>
#include <string>

using namespace std;

double calc(double A, double B, string operation)
{
    double calc = 0;                            // Буфер под результат
    char op = operation[0];                     // Операция в char для switch

    // Вычисление результата в switch.
    // Операция по умолчанию '+'
    switch (op)
        {
            case '-':
                calc = A - B;
                break;
            case '*':
                calc = A * B;
                break;
            case '/':
                calc = A / B;
                break;
            default:
                calc = A + B;
        }
    
    return calc;
}

int main() 
{
    const string support_operation = "+-*/";    // Поддерживаемые операции

    double A, B;                                // Числа на вводе
    string operation;                           // Арифметическая операция
    string err_msg = "";                        // Буфер ошибок

    // Ввод исходных данных
    cin >> A >> operation >> B;

    // Проверка корректности ввода и возможности вычисления результата
    if (cin.fail())
        {
        err_msg = "Incorrect input";
        }
    else if ((operation.length() != 1) 
        || operation.find_first_not_of(support_operation) != string::npos)
        {
        err_msg = "Incorrect operation";
        }
    
    // Если пустой буфер ошибок - вычисление значения и вывод в cout, иначе - вывод ошибки в cerr
    if (err_msg.length() == 0)
        {
        cout << calc(A, B, operation) << endl;
        }
    else
        {
        cerr << err_msg << endl;
        }

}
