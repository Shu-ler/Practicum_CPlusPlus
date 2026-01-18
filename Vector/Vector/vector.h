#pragma once
#include <cassert>
#include <cstdlib>
#include <new>
#include <utility>

template <typename T>
class Vector {
public:

    /// Конструктор по умолчанию
    /// Инициализирует вектор нулевого размера и вместимости. 
    /// Не выбрасывает исключений
    /// Алгоритмическая сложность: O(1). 
    Vector() = default;

    /// Конструктор вектора заданного размера. 
    /// Вместимость созданного вектора равна его размеру, а элементы 
    /// проинициализированы значением по умолчанию для типа T. 
    /// Алгоритмическая сложность: O(размер вектора).
    explicit Vector(size_t size)
        : data_(Allocate(size))
        , capacity_(size)
        , size_(size) {
        size_t constructed = 0;
        try {
            for (size_t i = 0; i != size; ++i) {
                new (data_ + i) T();
                ++constructed;
            }
        }
        catch (...) {
            // Уничтожаем созданные объекты
            DestroyN(data_, constructed);

            // Освобождаем память, выделенную через Allocate
            Deallocate(data_);

            // Обнуляем данные вектора
            data_ = nullptr;
            capacity_ = size_ = 0;

            // Перевыбрасываем пойманное исключение
            throw;
        }
    }

    /// Копирующий конструктор. Создаёт копию элементов исходного вектора. 
    /// Имеет вместимость, равную размеру исходного вектора, то есть выделяет память без запаса. 
    /// Алгоритмическая сложность: O(размер исходного вектора).
    Vector(const Vector& other)
        : data_(Allocate(other.size_))
        , capacity_(other.size_)
        , size_(other.size_) {
        size_t constructed = 0;
        try {
            for (size_t i = 0; i != other.size_; ++i) {
                CopyConstruct(data_ + i, other.data_[i]);
                ++constructed;
            }
        }
        catch (...) {
            // Уничтожаем созданные объекты
            DestroyN(data_, constructed);

            // Освобождаем память, выделенную через Allocate
            Deallocate(data_);

            // Обнуляем данные вектора
            data_ = nullptr;
            capacity_ = size_ = 0;

            // Перевыбрасываем пойманное исключение
            throw;
        }
    }

    /// Деструктор. 
    /// Разрушает содержащиеся в векторе элементы и освобождает занимаемую ими память. 
    /// Алгоритмическая сложность: O(размер вектора).
    ~Vector() {
        DestroyN(data_, size_);
        Deallocate(data_);
    }

    size_t Size() const noexcept {
        return size_;
    }

    size_t Capacity() const noexcept {
        return capacity_;
    }

    /// Резервирует достаточно места, чтобы вместить количество элементов, равное capacity. 
    /// Если новая вместимость не превышает текущую, метод не делает ничего. 
    /// Алгоритмическая сложность: O(размер вектора).
    void Reserve(size_t new_capacity) {
        if (new_capacity <= capacity_) {
            return;
        }

        T* new_data = Allocate(new_capacity);
        size_t constructed = 0;
        try {
            for (size_t i = 0; i != size_; ++i) {
                CopyConstruct(new_data + i, data_[i]);
                ++constructed;
            }
        }
        catch (...) {
            // Уничтожаем скопированные объекты
            DestroyN(new_data, constructed);
            // Освобождаем память
            Deallocate(new_data);
            // Вектор не меняется
            throw;  // перевыбрасываем
        }

        // Успешно скопировали — разрушаем старые объекты и освобождаем память
        DestroyN(data_, size_);
        Deallocate(data_);

        data_ = new_data;
        capacity_ = new_capacity;
        // size_ остаётся прежним
    }

    const T& operator[](size_t index) const noexcept {
        return const_cast<Vector&>(*this)[index];
    }

    T& operator[](size_t index) noexcept {
        assert(index < size_);
        return data_[index];
    }

private:
    // Выделяет сырую память под n элементов и возвращает указатель на неё
    static T* Allocate(size_t n) {
        return n != 0 ? static_cast<T*>(operator new(n * sizeof(T))) : nullptr;
    }

    // Освобождает сырую память, выделенную ранее по адресу buf при помощи Allocate
    static void Deallocate(T* buf) noexcept {
        operator delete(buf);
    }

    // Вызывает деструкторы n объектов массива по адресу buf
    static void DestroyN(T* buf, size_t n) noexcept {
        for (size_t i = 0; i != n; ++i) {
            Destroy(buf + i);
        }
    }

    // Создаёт копию объекта elem в сырой памяти по адресу buf
    static void CopyConstruct(T* buf, const T& elem) {
        new (buf) T(elem);
    }

    // Вызывает деструктор объекта по адресу buf
    static void Destroy(T* buf) noexcept {
        buf->~T();
    }

private:
    T* data_ = nullptr;
    size_t capacity_ = 0;
    size_t size_ = 0;
};
