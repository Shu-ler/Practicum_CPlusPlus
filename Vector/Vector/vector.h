#pragma once
#include <cassert>
#include <cstdlib>
#include <new>
#include <utility>

template <typename T>
class RawMemory {
public:
    RawMemory() = default;

    explicit RawMemory(size_t capacity)
        : buffer_(Allocate(capacity))
        , capacity_(capacity) {
    }

    ~RawMemory() {
        Deallocate(buffer_);
    }

    T* operator+(size_t offset) noexcept {
        // Разрешается получать адрес ячейки памяти, следующей за последним элементом массива
        assert(offset <= capacity_);
        return buffer_ + offset;
    }

    const T* operator+(size_t offset) const noexcept {
        return const_cast<RawMemory&>(*this) + offset;
    }

    const T& operator[](size_t index) const noexcept {
        return const_cast<RawMemory&>(*this)[index];
    }

    T& operator[](size_t index) noexcept {
        assert(index < capacity_);
        return buffer_[index];
    }

    void Swap(RawMemory& other) noexcept {
        std::swap(buffer_, other.buffer_);
        std::swap(capacity_, other.capacity_);
    }

    const T* GetAddress() const noexcept {
        return buffer_;
    }

    T* GetAddress() noexcept {
        return buffer_;
    }

    size_t Capacity() const {
        return capacity_;
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

private:
    T* buffer_ = nullptr;
    size_t capacity_ = 0;
};

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
        : data_(size)
        , size_(size) {
        size_t constructed = 0;
        try {
            for (size_t i = 0; i != size; ++i) {
                new (data_ + i) T();
                ++constructed;
            }
        }
        catch (...) {
            DestroyN(data_.GetAddress(), constructed);
            throw;
        }
    }

    /// Копирующий конструктор. Создаёт копию элементов исходного вектора. 
    /// Имеет вместимость, равную размеру исходного вектора, то есть выделяет память без запаса. 
    /// Алгоритмическая сложность: O(размер исходного вектора).
    Vector(const Vector& other)
        : data_(other.size_)
        , size_(other.size_) {
        size_t constructed = 0;
        try {
            for (size_t i = 0; i != other.size_; ++i) {
                CopyConstruct(data_ + i, other.data_[i]);
                ++constructed;
            }
        }
        catch (...) {
            DestroyN(data_.GetAddress(), constructed);
            throw;
        }
    }

    /// Деструктор. 
    /// Разрушает содержащиеся в векторе элементы. 
    /// Алгоритмическая сложность: O(размер вектора).
    ~Vector() {
        DestroyN(data_.GetAddress(), size_);
    }

    size_t Size() const noexcept {
        return size_;
    }

    size_t Capacity() const noexcept {
        return data_.Capacity();
    }

    /// Резервирует достаточно места, чтобы вместить количество элементов, равное capacity. 
    /// Если новая вместимость не превышает текущую, метод не делает ничего. 
    /// Алгоритмическая сложность: O(размер вектора).
    void Reserve(size_t new_capacity) {
        if (new_capacity <= data_.Capacity()) {
            return;
        }

        RawMemory<T> new_data(new_capacity);
        size_t constructed = 0;
        try {
            for (size_t i = 0; i != size_; ++i) {
                CopyConstruct(new_data + i, data_[i]);
                ++constructed;
            }
        }
        catch (...) {
            DestroyN(new_data.GetAddress(), constructed);
            throw;
        }

        // Обмениваем текущие данные на новые
        data_.Swap(new_data);  // data_ = new_data, new_data = старый data_
        size_ = size_;  // размер не меняется
    }

    const T& operator[](size_t index) const noexcept {
        return const_cast<Vector&>(*this)[index];
    }

    T& operator[](size_t index) noexcept {
        assert(index < size_);
        return data_[index];
    }

private:
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
    RawMemory<T> data_;
    size_t size_ = 0;
};
