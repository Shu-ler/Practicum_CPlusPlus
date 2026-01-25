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
            Swap(rhs);
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
    //=====================================================
    // Конструкторы / деструктор
    //=====================================================

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
    
    //=====================================================
    // Присваивания
    //=====================================================

    /// Копирующее присваивание
    Vector& operator=(const Vector& rhs) {
        if (this != &rhs) {
            if (rhs.size_ <= data_.Capacity()) {
                AssignInPlace(rhs);
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

    //=====================================================
    // Итераторы
    //=====================================================
    using iterator = T*;
    using const_iterator = const T*;

    iterator begin() noexcept {
        return data_.GetAddress();
    }

    iterator end() noexcept {
        return data_.GetAddress() + size_;
    }

    const_iterator begin() const noexcept {
        return data_.GetAddress();
    }

    const_iterator end() const noexcept {
        return data_.GetAddress() + size_;
    }

    const_iterator cbegin() const noexcept {
        return data_.GetAddress();
    }

    const_iterator cend() const noexcept {
        return data_.GetAddress() + size_;
    }

    T& back() noexcept {
        assert(size_ > 0);
        return data_[size_ - 1];
    }

    const T& back() const noexcept {
        assert(size_ > 0);
        return data_[size_ - 1];
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

    /// Изменяет размер вектора
    void Resize(size_t new_size) {
        if (new_size <= size_) {
            std::destroy_n(data_.GetAddress() + new_size, size_ - new_size);
            size_ = new_size;
            return;
        }

        if (new_size <= data_.Capacity()) {
            std::uninitialized_value_construct_n(data_.GetAddress() + size_, new_size - size_);
            size_ = new_size;
            return;
        }

        // Нужно выделить новую память
        RawMemory<T> new_data(new_size);

        if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
            std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
        }
        else {
            std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
        }

        std::destroy_n(data_.GetAddress(), size_);
        data_.Swap(new_data);
        std::uninitialized_value_construct_n(data_.GetAddress() + size_, new_size - size_);
        size_ = new_size;
    }

    /// Добавляет элемент в конец (копирование)
    void PushBack(const T& value) {
        EmplaceBack(value);
    }

    /// Добавляет элемент в конец (перемещение)
    void PushBack(T&& value) {
        EmplaceBack(std::move(value));
    }

    /// Удаляет последний элемент 
    void PopBack() noexcept {
        assert(size_ > 0);
        --size_;
        std::destroy_at(data_.GetAddress() + size_);
    }

    /// Добавляет новый элемент в конец вектора, создавая его "на месте"
    /// с помощью переданных аргументов.
    template <typename... Args>
    T& EmplaceBack(Args&&... args) {
        // Если ёмкость позволяет — просто создаём элемент в конце
        if (size_ < data_.Capacity()) {
            T* result = new (data_.GetAddress() + size_) T(std::forward<Args>(args)...);
            ++size_;
            return *result;
        }

        // Иначе — нужно расширить вектор
        // Выделяем новую память: в 2 раза больше, или 1, если был пуст
        const size_t new_capacity = size_ == 0 ? 1 : size_ * 2;
        RawMemory<T> new_data(new_capacity);

        // Этап 1: копируем или перемещаем существующие элементы в новую память
        try {
            if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
                // Можно безопасно перемещать — используем move
                std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
            }
            else {
                // Приходится копировать (move может выбросить, а копирование — безопасный путь)
                std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
            }
        }
        catch (...) {
            // Если при копировании/перемещении старых элементов возникло исключение:
            // - уничтожаем все сконструированные объекты в new_data
            // - выбрасываем исключение дальше
            // Исходный вектор остаётся нетронутым
            std::destroy_n(new_data.GetAddress(), size_);
            throw;
        }

        // Этап 2: конструируем новый элемент в новой памяти
        T* result = nullptr;
        try {
            result = new (new_data.GetAddress() + size_) T(std::forward<Args>(args)...);
        }
        catch (...) {
            // Если при создании нового элемента возникло исключение:
            // - уничтожаем все старые элементы в new_data
            // - выбрасываем исключение
            // Исходный вектор не изменён
            std::destroy_n(new_data.GetAddress(), size_);
            throw;
        }

        // Этап 3: после успешного копирования и вставки —
        // разрушаем старые объекты и меняем буферы местами
        std::destroy_n(data_.GetAddress(), size_);
        data_.Swap(new_data);

        // Увеличиваем размер — операция завершена
        ++size_;

        return *result;
    }

    /// Emplace — вставка с perfect forwarding
    template <typename... Args>
    iterator Emplace(const_iterator pos, Args&&... args) {
        assert(pos >= begin() && pos <= end());

        if (size_ < data_.Capacity()) {
            return EmplaceWithoutReallocation(pos, std::forward<Args>(args)...);
        }
        else {
            return EmplaceWithReallocation(pos, std::forward<Args>(args)...);
        }
    }

    /// Insert — на основе Emplace
    iterator Insert(const_iterator pos, const T& value) {
        return Emplace(pos, value);
    }

    iterator Insert(const_iterator pos, T&& value) {
        return Emplace(pos, std::move(value));
    }

    /// Erase — удаление элемента
    iterator Erase(const_iterator pos) {
        assert(pos >= begin() && pos < end());
        T* const mutable_pos = const_cast<T*>(pos);

        // Перемещаем элементы влево
        std::move(mutable_pos + 1, end(), mutable_pos);

        // Уничтожаем "висячий" последний элемент
        --size_;
        std::destroy_at(data_.GetAddress() + size_);

        return mutable_pos;
    }

private:
    // Вставляет элемент в позицию 'pos' без перевыделения памяти
    template <typename... Args>
    iterator EmplaceWithoutReallocation(const_iterator pos, Args&&... args) {
        T* const insert_pos = const_cast<T*>(pos);
        const size_t insert_index = pos - begin();

        // Случай 1: вставка в конец — просто конструируем в конце
        if (pos == end()) {
            T* result = new (insert_pos) T(std::forward<Args>(args)...);
            ++size_;
            return result;
        }

        // Случай 2: вставка не в конец
        // 1. Создаём временный объект
        T temporary(std::forward<Args>(args)...);

        // 2. Создаём копию последнего элемента в позиции end()
        std::construct_at(end(), std::move(back()));

        try {
            // 3. Сдвигаем элементы вправо
            std::move_backward(begin() + insert_index, end() - 1, end());

            // 4. Перемещаем временный объект в pos
            data_[insert_index] = std::move(temporary);
        }
        catch (...) {
            std::destroy_at(end());
            throw;
        }
    
        ++size_;

        return insert_pos;
    }

    // Вставляет элемент в позицию 'pos' с перевыделением памяти
    template <typename... Args>
    iterator EmplaceWithReallocation(const_iterator pos, Args&&... args) {
        const size_t new_capacity = size_ == 0 ? 1 : size_ * 2;
        RawMemory<T> new_data(new_capacity);

        constexpr bool nothrow_move_or_no_copy =
            std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>;

        T* new_pos;
        try {
            if constexpr (nothrow_move_or_no_copy) {
                new_pos = std::uninitialized_move(begin(), const_cast<T*>(pos), new_data.GetAddress());
            }
            else {
                new_pos = std::uninitialized_copy(begin(), const_cast<T*>(pos), new_data.GetAddress());
            }
        }
        catch (...) {
            std::destroy_n(new_data.GetAddress(), pos - begin());
            throw;
        }

        T* result_addr;
        try {
            result_addr = new (new_pos) T(std::forward<Args>(args)...);
        }
        catch (...) {
            std::destroy_n(new_data.GetAddress(), pos - begin());
            throw;
        }

        try {
            if constexpr (nothrow_move_or_no_copy) {
                std::uninitialized_move(const_cast<T*>(pos), end(), new_pos + 1);
            }
            else {
                std::uninitialized_copy(const_cast<T*>(pos), end(), new_pos + 1);
            }
        }
        catch (...) {
            std::destroy_at(new_pos);
            std::destroy_n(new_data.GetAddress(), pos - begin());
            throw;
        }

        std::destroy_n(begin(), size_);
        data_.Swap(new_data);
        ++size_;

        return result_addr;
    }

    void AssignInPlace(const Vector<T>& rhs)
    {
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

private:
    RawMemory<T> data_;
    size_t size_ = 0;
};
