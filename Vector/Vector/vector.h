#pragma once
#include <cassert>
#include <cstdlib>
#include <new>
#include <utility>
#include <memory>
#include <type_traits>
#include <algorithm>

template <typename T>
class RawMemory {
public:
    /// Конструктор по умолчанию
    RawMemory() = default;

    /// Конструктор с размером
    explicit RawMemory(size_t capacity)
        : buffer_(Allocate(capacity))
        , capacity_(capacity) {
    }

    /// Копирующий конструктор
    RawMemory(const RawMemory&) = delete;
    
    /// Перемещающий конструктор
    RawMemory(RawMemory&& other) noexcept 
        : buffer_(other.buffer_)
        , capacity_(other.capacity_) {
        other.buffer_ = nullptr;
        other.capacity_ = 0;
    }

    /// Деструктор
    ~RawMemory() {
        Deallocate(buffer_);
    }

    /// Копирующее присваивание
    RawMemory& operator=(const RawMemory& rhs) = delete;

    /// Перемещающее присваивание
    RawMemory& operator=(RawMemory&& rhs) noexcept {
        if (this != &rhs) {
            T* const temp_buffer = rhs.buffer_;
            const size_t temp_capacity = rhs.capacity_;
            rhs.buffer_ = nullptr;
            rhs.capacity_ = 0;
            Deallocate(buffer_);
            buffer_ = temp_buffer;
            capacity_ = temp_capacity;
        }
        return *this;
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
        std::uninitialized_value_construct_n(data_.GetAddress(), size_);
    }

    /// Копирующий конструктор. Создаёт копию элементов исходного вектора. 
    /// Имеет вместимость, равную размеру исходного вектора, то есть выделяет память без запаса. 
    /// Алгоритмическая сложность: O(размер исходного вектора).
    Vector(const Vector& other)
        : data_(other.size_)
        , size_(other.size_) {
        std::uninitialized_copy_n(other.data_.GetAddress(), size_, data_.GetAddress());
    }

    /// Перемещающий конструктор
    /// O(1), noexcept
    /// other остаётся в валидном, но пустом состоянии
    Vector(Vector&& other) noexcept
        : data_(std::move(other.data_))
        , size_(other.size_) {
        other.size_ = 0;
    }

    /// Деструктор. 
    /// Разрушает содержащиеся в векторе элементы. 
    /// Алгоритмическая сложность: O(размер вектора).
    ~Vector() {
        std::destroy_n(data_.GetAddress(), size_);
    }
    
    /// Копирующее присваивание
    Vector& operator=(const Vector& rhs) {
        if (this != &rhs) {
            if (rhs.size_ <= data_.Capacity()) {
                // Есть место — можно скопировать "на месте"
                const size_t min_size = std::min(size_, rhs.size_);
                // Копируем общую часть
                std::copy_n(rhs.data_.GetAddress(), min_size, data_.GetAddress());
                // Если rhs больше — инициализируем новые
                if (rhs.size_ > size_) {
                    std::uninitialized_copy_n(rhs.data_.GetAddress() + min_size,
                        rhs.size_ - min_size,
                        data_.GetAddress() + min_size);
                }
                else {
                    // rhs меньше — уничтожаем лишние
                    std::destroy_n(data_.GetAddress() + rhs.size_, size_ - rhs.size_);
                }
            }
            else {
                // Мало места — делаем copy-and-swap
                Vector temp(rhs);
                Swap(temp);
            }
            size_ = rhs.size_;
        }
        return *this;
    }

    /// Перемещающее присваивание
    /// O(1), noexcept
    Vector& operator=(Vector&& rhs) noexcept {
        if (this != &rhs) {
            Swap(rhs);
        }
        return *this;
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

        if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
            // Можно безопасно перемещать
            std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
        }
        else {
            // Приходится копировать (move может выбросить, а копирование — безопасный путь)
            std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
        }

        // Уничтожаем старые объекты
        std::destroy_n(data_.GetAddress(), size_);
        // Меняем местами буферы
        data_.Swap(new_data);
        // size_ остаётся прежним
    }

    /// Обменивает всё содержимое
    /// O(1), noexcept
    void Swap(Vector& other) noexcept {
        data_.Swap(other.data_);
        std::swap(size_, other.size_);
    }

    const T& operator[](size_t index) const noexcept {
        return const_cast<Vector&>(*this)[index];
    }

    T& operator[](size_t index) noexcept {
        assert(index < size_);
        return data_[index];
    }

private:
    RawMemory<T> data_;
    size_t size_ = 0;
};
