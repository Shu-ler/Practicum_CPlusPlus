#include <cassert>
#include <string>
#include <string_view>

using namespace std::literals;

// Шаблонная функция, возвращающая true, если первый аргумент равен хотя бы одному из остальных
template <typename T, typename... Args>
bool EqualsToOneOf(const T& value, const Args&... args) {
    return ((value == args) || ...);  // Свёртка с оператором ||
}

int main() {
    assert(EqualsToOneOf("hello"sv, "hi"s, "hello"s));
    assert(!EqualsToOneOf(1, 10, 2, 3, 6));
    assert(!EqualsToOneOf(8));
}