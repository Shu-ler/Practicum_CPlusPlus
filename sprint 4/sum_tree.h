#pragma once
#include <vector>

// Это структура данных дерево.
// В ней хранится значение и набор поддеревьев (может быть пустым).
template<class T>
struct Tree {
    T value;
    std::vector<Tree> sibs{};
};

// Реализуйте эту функцию.
template<class T>
T SumTree(const Tree<T>& tree) {
    T res = tree.value;
    for (const auto& knot : tree.sibs) {
        res += SumTree(knot);
    }
    return res;
}
