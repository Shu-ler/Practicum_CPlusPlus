#include <cassert>
#include <cctype>
#include <string>

/// @brief Возвращает букву в другом регистре. Проверка, буква ли это, не производится!
/// @param c Исходная буква
/// @return Буква в измененном регистре
char ChangeLetterСase(char c)
{
    return std::isupper(c) ? static_cast<char>(std::tolower(c)) : static_cast<char>(std::toupper(c));
}

/// @brief Возвращает измененный символ (изменение регистра буквы)
/// @param c Исходный символ
/// @return Измененый символ
char SwitchCase(char c)
{
    return (std::isalpha(c)) ? ChangeLetterСase(c) : c;
}

/// @brief Преобразует строку (изменение регистра букв)
/// @param text Исходная строка
/// @return Измененная строка
std::string SwitchCase(std::string text)
{
    for (char &c : text)
    {
        c = SwitchCase(c);
    }
    return text;
}

void TestLetter()
{
    assert(SwitchCase('a') == 'A');
    assert(SwitchCase('b') == 'B');
    assert(SwitchCase('e') == 'E');
    assert(SwitchCase('x') == 'X');

    assert(SwitchCase('A') == 'a');
    assert(SwitchCase('B') == 'b');
    assert(SwitchCase('E') == 'e');
    assert(SwitchCase('X') == 'x');
}

void TestNoLetter()
{
    assert(SwitchCase('!') == '!');
    assert(SwitchCase('-') == '-');
    assert(SwitchCase(' ') == ' ');
    assert(SwitchCase('\0') == '\0');
}

void TestString()
{
    assert(SwitchCase("hello") == "HELLO");
    assert(SwitchCase("HELLO") == "hello");
    assert(SwitchCase("Hello") == "hELLO");
    assert(SwitchCase("hELLO") == "Hello");

    assert(SwitchCase("I love C++!") == "i LOVE c++!");
    assert(SwitchCase("i LOVE c++!") == "I love C++!");
}

int main()
{

    TestLetter();

    TestNoLetter();

    TestString();
}