#include <cassert>
#include <iostream>

int CharToDigit(char ch)
{
    int Digit = 0;

    if (ch >= '0' && ch <= '9')
    {
        Digit = ch - '0';
    }
    else if (ch >= 'a' && ch <= 'z')
    {
        Digit = ch - 'a' + 10;
    }
    else if (ch >= 'A' && ch <= 'Z')
    {
        Digit = ch - 'A' + 10;
    }

    return Digit;
}

int StringToNumber(std::string s, int base)
{
    int Digit = 0;

    size_t string_size = s.size();

    for (size_t i = 0; i < string_size; i++)
    {
        Digit = Digit * base + CharToDigit(s[i]);
    }

    return Digit;
}

int main()
{
    std::string src_string;
    int src_base;

    std::cin >> src_string >> src_base;

    // Переводим число из строкового представления в системе
    // счисления src_base в int.
    int number = StringToNumber(src_string, src_base);

    // Выводим число в десятичной системе счисления.
    std::cout << number << std::endl;
}
