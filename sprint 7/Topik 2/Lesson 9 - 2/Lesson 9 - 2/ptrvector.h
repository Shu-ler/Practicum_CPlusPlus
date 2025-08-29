#pragma once

#include <vector>

template <typename T>
class PtrVector {
public:
    PtrVector() = default;

    // Создаёт вектор указателей на копии объектов из other
    PtrVector(const PtrVector& other) {
        // Резервируем место в vector-е для хранения нужного количества элементов
        // Благодаря этому при push_back не будет выбрасываться исключение
        items_.reserve(other.items_.size());

        try {
            for (auto p : other.items_) {
                // Копируем объект, если указатель на него ненулевой
                auto p_copy = p ? new T(*p) : nullptr;  // new может выбросить исключение

                // Не выбросит исключение, т. к. в vector память уже зарезервирована
                items_.push_back(p_copy);
            }
        }
        catch (...) {
            // удаляем элементы в векторе и перевыбрасываем пойманное исключение
            DeleteItems();
            throw;
        }
    }

    // Деструктор удаляет объекты в куче, на которые ссылаются указатели,
    // в векторе items_
    ~PtrVector() {
        DeleteItems();
    }

    // Возвращает ссылку на вектор указателей
    std::vector<T*>& GetItems() noexcept {
        return items_;
    }

    // Возвращает константную ссылку на вектор указателей
    std::vector<T*> const& GetItems() const noexcept {
        return items_;
    }

    const T* GetItem(size_t index) const noexcept {
        if (index >= items_.size()) return nullptr;
        return items_[index];
    }

    const T* at(size_t index) const noexcept {
        if (index >= items_.size()) return nullptr;
        return items_[index];
    }

    T* at(size_t index) noexcept {
        if (index >= items_.size()) return nullptr;
        return items_[index];
    }

    // Добавляет элемент в конец вектора
    void PushBack(T* item) {
        items_.push_back(item);
    }

    // Возвращает количество элементов вектора
    size_t Size() const noexcept {
        return items_.size();
    }

private:
    void DeleteItems() noexcept {
        for (auto p : items_) {
            delete p;
        }
    }

    std::vector<T*> items_;
};
