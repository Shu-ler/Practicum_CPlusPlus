#pragma once

template <typename T>
class Optional {
public:
    // Напишите следующие методы.
    Optional() = default;
    Optional(const T& value)
        : value_(value),
        has_value_(true) {}
    
    // Возвращает true, если класс содержит значение.
    bool HasValue() const {
        return has_value_;
    }
    
    // Возвращает значение.
    T GetValue() const {
        return value_;
    }
    
    // Устанавливает новое значение.
    void SetValue(T value) {
        value_ = value;
        has_value_ = true;
    }
    
    // Удаляет значение.
    void Reset() {
        has_value_ = false;
    }
    
private:
    T value_;
    bool has_value_ = false;
};
