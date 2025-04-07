#include <iostream>
#include <string>
#include <cmath>

using namespace std;


bool NumInput(double& numbuffer)
{

}

int main()
{
    // Сообщения об ошибках
    const str error_num = "Error: Numeric operand expected";
    const str error_op = "Error: Unknown token ";
    
    // Команды калькулятора
    const str command_plus = "+";
    const str command_quit = "q";

    // Переменные
    double number;      // Число в калькуляторе
    string command = ""; // Вводимая команда

    if (!(std::cin >> number))
    {
        cerr << error_num << endl;
    }
    else
    {
        while (command != command_quit)
        {
            cin >> in_buf;
            if (command == command_plus)
            {
                /* code */
            }
            else if (command != command_quit)
            {
                cerr << error_op << command << endl;
                break;
            }
            
            
        }
        
    }
}