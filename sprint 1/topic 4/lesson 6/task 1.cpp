#include <iostream>
#include <string>
#include <vector>

int main()
{
    using namespace std::literals;

    const std::vector<std::string> num_names = {
        "zero"s, "one"s, "two"s, "three"s, "four"s,
        "five"s, "six"s, "seven"s, "eight"s, "nine"s,
        "ten"s, "eleven"s, "twelve"s, "thirteen"s, "fourteen"s,
        "fifteen"s, "sixteen"s, "seventeen"s, "eighteen"s, "nineteen"s};

    const std::vector<std::string> tens_names = {
        "twenty"s, "thirty"s, "forty"s, "fifty"s,
        "sixty"s, "seventy"s, "eighty"s, "ninety"s};

    int number;      // Буфер ввода
    std::string num; // Буфер вывода

    // Ввод числа
    std::cin >> number;

    // Вычисление единиц и десятков как остаток от деления на 10 и частное:
    int units = number % 10; // Единицы.
    int tens = number / 10;  // Десятки.

    // Формирование числа прописью
    if (number < 20)
    {
        num = num_names[number];
    }
    else
    {
        num = tens_names[tens - 2] + ((units == 0) ? ""s : "-"s + num_names[units]);
    }

    // Вывод результата
    std::cout << num << std::endl;
}
