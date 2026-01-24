// Шаблонный класс Optional, реализующий семантику "значение или пусто"
// Аналог std::optional, но с ручным управлением памятью через placement new / explicit destructor

#include <stdexcept>
#include <utility>
#include <type_traits>
#include <cassert>

/**
 * @brief Исключение, выбрасываемое при попытке доступа к значению в пустом Optional.
 */
class BadOptionalAccess : public std::exception {
public:
    using std::exception::exception;

    /**
     * @brief Возвращает сообщение об ошибке.
     * @return C-строка с описанием ошибки.
     */
    const char* what() const noexcept override {
        return "Bad optional access";
    }
};

/**
 * @brief Шаблонный класс, хранящий либо значение типа T, либо состояние "пусто".
 *
 * Реализует семантику, аналогичную std::optional.
 * Хранит значение на стеке (в выровненной сырой памяти), а не в динамической памяти.
 * Управляет жизненным циклом объекта вручную:
 * - инициализация через placement new
 * - уничтожение через явный вызов деструктора
 *
 * Поддерживает:
 * - конструкторы копирования и перемещения
 * - операторы присваивания
 * - доступ к значению через * и ->
 * - безопасный доступ с проверкой (Value())
 * - сброс значения (Reset)
 */
template <typename T>
class Optional {
public:
    //=====================================================
    // Конструкторы
    //=====================================================

    /// Конструктор по умолчанию: создаёт пустой Optional
    Optional() noexcept = default;

    /// Конструктор от значения: копирует объект в внутреннюю память
    Optional(const T& value) {
        new (data_) T(value);
        is_initialized_ = true;
    }

    /// Конструктор от rvalue: перемещает объект в внутреннюю память
    Optional(T&& value) {
        new (data_) T(std::move(value));
        is_initialized_ = true;
    }

    /// Копирующий конструктор
    Optional(const Optional& other) {
        if (other.is_initialized_) {
            new (data_) T(*other);
            is_initialized_ = true;
        }
        // если other пуст, is_initialized_ остаётся false
    }

    /// Перемещающий конструктор
    /// Помечает исходный объект как пустой, не уничтожая объект
    Optional(Optional&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
        : is_initialized_(other.is_initialized_) {
        if (other.is_initialized_) {
            new (data_) T(std::move(*other));  // перемещаем значение
            other.is_initialized_ = false;     // other становится пустым
        }
    }

    //=====================================================
    // Операторы присваивания
    //=====================================================

    /// Присваивание значения: если инициализирован — присваивает, иначе создаёт
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

    /// Присваивание rvalue: аналогично, с перемещением
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

    /// Копирующее присваивание
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
            Reset();  // rhs пуст — делаем this пустым
        }
        return *this;
    }

    /// Перемещающее присваивание
    /// Если rhs инициализирован, перемещает его значение и помечает как пустой
    Optional& operator=(Optional&& rhs) noexcept(
        std::is_nothrow_move_assignable_v<T>&& std::is_nothrow_move_constructible_v<T>) {
        if (this == &rhs) {
            return *this;
        }

        if (rhs.is_initialized_) {
            if (is_initialized_) {
                **this = std::move(*rhs);  // присваиваем значение
            }
            else {
                new (data_) T(std::move(*rhs));  // создаём новое
                is_initialized_ = true;
            }
            rhs.is_initialized_ = false;  // rhs больше не владеет объектом
        }
        else {
            Reset();  // rhs пуст — сбрасываем this
        }
        return *this;
    }

    //=====================================================
    // Деструктор
    //=====================================================

    /// Деструктор: корректно уничтожает значение, если оно есть
    ~Optional() {
        Reset();
    }

    //=====================================================
    // Проверка состояния
    //=====================================================

    /// Проверяет, содержит ли Optional значение
    bool HasValue() const noexcept {
        return is_initialized_;
    }

    //=====================================================
    // Доступ к значению
    //=====================================================

    /// Разыменование: возвращает ссылку на значение
    /// Бросает BadOptionalAccess, если Optional пуст
    T& operator*()& {
        assert(is_initialized_ && "Dereferencing empty Optional");
        return *reinterpret_cast<T*>(data_);
    }

    /// Константное разыменование
    const T& operator*() const& {
        assert(is_initialized_ && "Dereferencing empty Optional");
        return *reinterpret_cast<const T*>(data_);
    }

    /// Доступ к членам: возвращает указатель на значение
    T* operator->() {
        return std::addressof(**this);
    }

    /// Константный доступ к членам
    const T* operator->() const {
        return std::addressof(**this);
    }

    /// Доступ к значению с проверкой
    /// Бросает BadOptionalAccess, если Optional пуст
    T& Value()& {
        CheckHasValue();
        return *reinterpret_cast<T*>(data_);
    }

    /// Константный доступ с проверкой
    const T& Value() const& {
        CheckHasValue();
        return *reinterpret_cast<const T*>(data_);
    }

    //=====================================================
    // Управление значением
    //=====================================================

    /// Удаляет значение, вызывая деструктор, если оно есть
    void Reset() noexcept {
        if (is_initialized_) {
            reinterpret_cast<T*>(data_)->~T();
            is_initialized_ = false;
        }
    }

    /// Создаёт значение "на месте", разрушая старое, если оно было
    template <typename... Args>
    void Emplace(Args&&... args) {
        if (is_initialized_) {
            Reset();
        }
        new (data_) T(std::forward<Args>(args)...);
        is_initialized_ = true;
    }

private:
    /// Проверяет, что Optional инициализирован. Бросает исключение, если нет.
    void CheckHasValue() const {
        if (!is_initialized_) {
            throw BadOptionalAccess();
        }
    }

private:
    /// Выровненная сырая память для хранения объекта типа T
    alignas(T) char data_[sizeof(T)];

    /// Флаг, указывающий, инициализирован ли объект
    bool is_initialized_ = false;
};
