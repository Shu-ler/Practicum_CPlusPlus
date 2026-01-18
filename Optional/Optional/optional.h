// optional.h
#include <stdexcept>
#include <utility>
#include <type_traits>

// Исключение для доступа к пустому Optional
class BadOptionalAccess : public std::exception {
public:
    using std::exception::exception;

    const char* what() const noexcept override {
        return "Bad optional access";
    }
};

template <typename T>
class Optional {
public:
    // Конструкторы
    Optional() noexcept = default;

    Optional(const T& value) {
        new (data_) T(value);
        is_initialized_ = true;
    }

    Optional(T&& value) {
        new (data_) T(std::move(value));
        is_initialized_ = true;
    }

    Optional(const Optional& other) {
        if (other.is_initialized_) {
            new (data_) T(*other);
            is_initialized_ = true;
        }
        // если пустой — оставляем is_initialized_ = false
    }

    // Move-конструктор
    Optional(Optional&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
        : is_initialized_(other.is_initialized_) {
        if (other.is_initialized_) {
            new (data_) T(std::move(*other));
            other.is_initialized_ = false;
        }
    }

    // Операторы присваивания
    Optional& operator=(const T& value) {
        if (is_initialized_) {
            reinterpret_cast<T*>(data_)->operator=(value);
        }
        else {
            new (data_) T(value);
            is_initialized_ = true;
        }
        return *this;
    }

    Optional& operator=(T&& rhs) {
        if (is_initialized_) {
            reinterpret_cast<T*>(data_)->operator=(std::move(rhs));
        }
        else {
            new (data_) T(std::move(rhs));
            is_initialized_ = true;
        }
        return *this;
    }

    Optional& operator=(const Optional& rhs) {
        if (this == &rhs) {
            return *this;
        }

        if (rhs.is_initialized_) {
            if (is_initialized_) {
                reinterpret_cast<T*>(data_)->operator=(*rhs);
            }
            else {
                new (data_) T(*rhs);
                is_initialized_ = true;
            }
        }
        else {
            Reset();
        }
        return *this;
    }

    Optional& operator=(Optional&& rhs) noexcept(
        std::is_nothrow_move_assignable_v<T>&& std::is_nothrow_move_constructible_v<T>) {
        if (this == &rhs) {
            return *this;
        }

        if (rhs.is_initialized_) {
            if (is_initialized_) {
                **this = std::move(*rhs);
            }
            else {
                new (data_) T(std::move(*rhs));
                is_initialized_ = true;
            }
            rhs.is_initialized_ = false;
        }
        else {
            Reset();
        }
        return *this;
    }

    // Деструктор
    ~Optional() {
        Reset();  // безопасно: если не инициализирован — ничего не делает
    }

    // Проверка наличия значения
    bool HasValue() const noexcept {
        return is_initialized_;
    }

    // Операторы разыменования
    T& operator*()& {
        if (!is_initialized_) {
            throw BadOptionalAccess();
        }
        return *reinterpret_cast<T*>(data_);
    }

    const T& operator*() const& {
        if (!is_initialized_) {
            throw BadOptionalAccess();
        }
        return *reinterpret_cast<const T*>(data_);
    }

    T* operator->() {
        return std::addressof(**this);
    }

    const T* operator->() const {
        return std::addressof(**this);
    }

    // Доступ к значению с проверкой
    T& Value()& {
        if (!is_initialized_) {
            throw BadOptionalAccess();
        }
        return *reinterpret_cast<T*>(data_);
    }

    const T& Value() const& {
        if (!is_initialized_) {
            throw BadOptionalAccess();
        }
        return *reinterpret_cast<const T*>(data_);
    }

    // Сброс значения
    void Reset() noexcept {
        if (is_initialized_) {
            reinterpret_cast<T*>(data_)->~T();
            is_initialized_ = false;
        }
    }

private:
    alignas(T) char data_[sizeof(T)];
    bool is_initialized_ = false;
};

// Специализация для void — не требуется
