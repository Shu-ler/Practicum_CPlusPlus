#include <iostream>
#include <string>
#include <string_view>
#include <memory>
#include <utility>

using namespace std;

// Базовый абстрактный класс для хранения любого типа
class AnyStorageBase {
public:
    virtual ~AnyStorageBase() = default;                    // Виртуальный деструктор
    virtual void Print(ostream& out) const = 0;             // Чистый виртуальный метод
};

// Шаблонный класс, хранящий значение типа T
template <typename T>
class AnyStorage : public AnyStorageBase {
public:
    // Конструктор с forwarding reference
    template <typename U>
    AnyStorage(U&& value) : data_(std::forward<U>(value)) {}

    void Print(ostream& out) const override {
        out << data_;
    }

private:
    T data_;
};

// Основной класс Any — не шаблонный
class Any {
public:
    // Шаблонный конструктор — forwarding reference
    template <typename S>
    Any(S&& value) {
        using Initial = std::remove_reference_t<S>;
        ptr_ = std::make_unique<AnyStorage<Initial>>(std::forward<S>(value));
    }

    // Вывод через делегирование
    void Print(std::ostream& out) const {
        ptr_->Print(out);
    }

private:
    std::unique_ptr<AnyStorageBase> ptr_;
};

class Dumper {
public:
    Dumper() {
        std::cout << "construct"sv << std::endl;
    }
    ~Dumper() {
        std::cout << "destruct"sv << std::endl;
    }
    Dumper(const Dumper&) {
        std::cout << "copy"sv << std::endl;
    }
    Dumper(Dumper&&) {
        std::cout << "move"sv << std::endl;
    }
    Dumper& operator=(const Dumper&) {
        std::cout << "= copy"sv << std::endl;
        return *this;
    }
    Dumper& operator=(Dumper&&) {
        std::cout << "= move"sv << std::endl;
        return *this;
    }
};

// Оператор вывода в поток
ostream& operator<<(ostream& out, const Any& arg) {
    arg.Print(out);
    return out;
}

// Специализация для Dumper (ничего не выводит)
ostream& operator<<(ostream& out, const Dumper&) {
    return out;
}

int main() {
    Any any_int(42);
    Any any_string("abc"s);

    // операция вывода Any в поток определена чуть выше в примере
    cout << any_int << endl << any_string << endl;

    Any any_dumper_temp{ Dumper{} };

    Dumper auto_dumper;
    Any any_dumper_auto(auto_dumper);
}