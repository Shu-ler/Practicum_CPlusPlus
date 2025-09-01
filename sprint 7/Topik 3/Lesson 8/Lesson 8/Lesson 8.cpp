#include <cassert>
#include <cstdlib>

template <typename Type>
class ArrayPtr {
public:
    // Инициализирует ArrayPtr нулевым указателем
    ArrayPtr() = default;

    // Создаёт в куче массив из size элементов типа Type.
    // Если size == 0, поле raw_ptr_ должно быть равно nullptr
    explicit ArrayPtr(size_t size) {
        // TODO: реализуйте конструктор самостоятельно
    }

    // Конструктор из сырого указателя, хранящего адрес массива в куче либо nullptr
    explicit ArrayPtr(Type* raw_ptr) noexcept {
        // TODO: реализуйте конструктор самостоятельно
    }

    // Запрещаем копирование
    ArrayPtr(const ArrayPtr&) = delete;

    ~ArrayPtr() {
        // Напишите деструктор самостоятельно
    }

    // Запрещаем присваивание
    ArrayPtr& operator=(const ArrayPtr&) = delete;

    // Прекращает владением массивом в памяти, возвращает значение адреса массива
    // После вызова метода указатель на массив должен обнулиться
    [[nodiscard]] Type* Release() noexcept {
        // Заглушка. TODO: реализуйте метод самостоятельно
        return nullptr;
    }

    // Возвращает ссылку на элемент массива с индексом index
    Type& operator[](size_t index) noexcept {
        // TODO: реализуйте операцию самостоятельно
    }

    // Возвращает константную ссылку на элемент массива с индексом index
    const Type& operator[](size_t index) const noexcept {
        // TODO: реализуйте операцию самостоятельно
    }

    // Возвращает true, если указатель ненулевой, и false в противном случае
    explicit operator bool() const {
        // Заглушка. TODO: реализуйте операцию самостоятельно
        return false;
    }

    // Возвращает значение сырого указателя, хранящего адрес начала массива
    Type* Get() const noexcept {
        // Заглушка. TODO: реализуйте метод самостоятельно
        return nullptr;
    }

    // Обменивается значениям указателя на массив с объектом other
    void swap(ArrayPtr& other) noexcept {
        // TODO: реализуйте метод самостоятельно
    }

private:
    Type* raw_ptr_ = nullptr;
};

int main() {
    ArrayPtr<int> numbers(10);
    const auto& const_numbers = numbers;

    numbers[2] = 42;
    assert(const_numbers[2] == 42);
    assert(&const_numbers[2] == &numbers[2]);

    assert(numbers.Get() == &numbers[0]);

    ArrayPtr<int> numbers_2(5);
    numbers_2[2] = 43;

    numbers.swap(numbers_2);

    assert(numbers_2[2] == 42);
    assert(numbers[2] == 43);
}
