#include <cassert>
#include <cstddef>  // нужно для nullptr_t

using namespace std;

// Реализуйте шаблон класса UniquePtr
template <typename T>
class UniquePtr {
public:
    // Конструктор по умолчанию — пустой указатель
    UniquePtr()
        : ptr_(nullptr) {
    }

    // Конструктор, захватывающий указатель
    explicit UniquePtr(T* ptr)
        : ptr_(ptr) {
    }

    // Конструктор копирования - запрещаем
    UniquePtr(const UniquePtr&) = delete;

    // Конструктор перемещения
    UniquePtr(UniquePtr&& other) : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    // Оператор копирования - запрещаем
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Оператор присваивания с nullptr
    UniquePtr& operator=(nullptr_t) {
        Reset(nullptr);
        return *this;
    }

    // Move-оператор присваивания
    UniquePtr& operator=(UniquePtr&& other) {
        if (this != &other) {
            Reset(other.ptr_);
            other.ptr_ = nullptr;
        }
        return *this;
    }
    
    // Деструктор - удаление объекта владения
    ~UniquePtr() {
        delete ptr_;
    }

    // Оператор разыменования
    T& operator*() const {
        assert(ptr_ != nullptr);
        return *ptr_;
    }
    
    // Оператор стрелки
    T* operator->() const {
        assert(ptr_ != nullptr);
        return ptr_;
    }

    // Release: отдаёт указатель, перестаёт им владеть
    T* Release() {
        T* raw_ptr = ptr_;
        ptr_ = nullptr;
        return raw_ptr;
    }

    // Reset: освобождает старый указатель, захватывает новый
    void Reset(T* ptr) {
        if (ptr_ != ptr) {
            delete ptr_;
            ptr_ = ptr;
        }
    }

    // Swap: обмен с другим UniquePtr
    void Swap(UniquePtr& other) {
        T* temp = ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = temp;
    }
    
    // Get: возвращает указатель (не отбирая владение)
    T* Get() const {
        return ptr_;
    }

private:
    T* ptr_;
};

struct Item {
    static int counter;
    int value;
    Item(int v = 0)
        : value(v)
    {
        ++counter;
    }
    Item(const Item& other)
        : value(other.value)
    {
        ++counter;
    }
    ~Item() {
        --counter;
    }
};

int Item::counter = 0;

void TestLifetime() {
    Item::counter = 0;
    {
        UniquePtr<Item> ptr(new Item);
        assert(Item::counter == 1);

        ptr.Reset(new Item);
        assert(Item::counter == 1);
    }
    assert(Item::counter == 0);

    {
        UniquePtr<Item> ptr(new Item);
        assert(Item::counter == 1);

        auto rawPtr = ptr.Release();
        assert(Item::counter == 1);

        delete rawPtr;
        assert(Item::counter == 0);
    }
    assert(Item::counter == 0);
}

void TestGetters() {
    UniquePtr<Item> ptr(new Item(42));
    assert(ptr.Get()->value == 42);
    assert((*ptr).value == 42);
    assert(ptr->value == 42);
}

int main() {
    TestLifetime();
    TestGetters();
}
