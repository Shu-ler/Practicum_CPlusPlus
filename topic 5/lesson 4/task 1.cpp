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

int main()
{
    char ch;
    std::cin >> ch;

    // Переводим цифру из char в int.
    const int digit_value = CharToDigit(ch);

    std::cout << digit_value << std::endl;
}
