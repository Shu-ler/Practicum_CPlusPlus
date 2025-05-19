#pragma once

// Напишите здесь реализацию структур.
template<class T, class U>
struct IsSameType
{
    static constexpr bool value = false;
};

template<class T>
struct IsSameType<T, T>
{
    static constexpr bool value = true;
};

// Шаблонная переменная позволяет использовать
// IsSameTypeV<int, int> вместо IsSameType<int, int>::value.
template<class T, class U>
inline constexpr bool IsSameTypeV = IsSameType<T, U>::value;
