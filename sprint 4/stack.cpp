#include <cstdlib>
#include <iostream>
#include <stack>
#include <string>

// Вспомогательные функции для работы со скобками.
// Можно их использовать, можно написать по-своему.

// Возвращает true для открывающих скобок.
bool IsOpeningParenthesis(const char c) {
    return c == '(' || c == '{' || c == '[';
}

// Возвращает парную закрывающую скобку для заданной открывающей скобки.
char ClosingParenthesis(const char c) {
    if (c == '(') {
        return ')';
    }
    if (c == '{') {
        return '}';
    }
    if (c == '[') {
        return ']';
    }
    std::abort(); // Неверный вызов функции.
}

// Реализуйте эту функцию.
bool IsValidSequence(const std::string& str) {
    std::stack<char> opening;

    for (const auto c : str) {
        if (IsOpeningParenthesis(c)) {
            opening.push(ClosingParenthesis(c));
        } else if (opening.empty() || opening.top() != c) {
            return false;
        } else {
            opening.pop();
        }
    }
    return opening.empty();
}

using namespace std::literals;

int main() {
    std::string sequence;
    std::cin >> sequence;
    std::cout << (IsValidSequence(sequence) ? "Да"s : "Нет"s) << std::endl;
}
