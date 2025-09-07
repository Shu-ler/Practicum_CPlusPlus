#include <stdexcept>

// Умный указатель, удаляющий связанный объект при своём разрушении.
// Параметр шаблона T задаёт тип объекта, на который ссылается указатель
template <typename T>
class ArrayPtr {
public:
	// Конструктор по умолчанию создаёт нулевой указатель,
	// так как поле ptr_ имеет значение по умолчанию nullptr
	ArrayPtr() = default;

	// Создаёт указатель, ссылающийся на переданный raw_ptr.
	// raw_ptr ссылается либо на объект, созданный в куче при помощи new,
	// либо является нулевым указателем
	// Спецификатор noexcept обозначает, что метод не бросает исключений
	explicit ArrayPtr(T* raw_ptr) noexcept {
		ptr_ = raw_ptr;
	}

	explicit ArrayPtr(size_t size)
		: ptr_(size != 0 ? new T[size] : nullptr) {
	}

	// Удаляем у класса конструктор копирования
	ArrayPtr(const ArrayPtr&) = delete;

	// Деструктор. Удаляет объект, на который ссылается умный указатель.
	~ArrayPtr() {
		delete ptr_;
	}

	// Оператор приведения к типу bool позволяет узнать, ссылается ли умный указатель
	// на какой-либо объект
	explicit operator bool() const noexcept {
		return ptr_ != nullptr;
	}

	// Оператор разыменования возвращает ссылку на объект
	// Выбрасывает исключение std::logic_error, если указатель нулевой
	T& operator*() const {
		if (ptr_ == nullptr) {
			throw std::logic_error("Null pointer!");
		}
		return *ptr_;
	}

	// Оператор -> должен возвращать указатель на объект
	// Выбрасывает исключение std::logic_error, если указатель нулевой
	T* operator->() const {
		if (ptr_ == nullptr) {
			throw std::logic_error("Null pointer!");
		}
		return ptr_;
	}

	// Возвращает указатель, хранящийся внутри ScopedPtr
	T* GetRawPtr() const noexcept {
		return ptr_;
	}

	// Прекращает владение объектом, на который ссылается умный указатель.
	// Возвращает прежнее значение "сырого" указателя и устанавливает поле ptr_ в null
	T* Release() noexcept {
		T* tmp = ptr_;
		ptr_ = nullptr;
		return tmp;
	}

private:
	T* ptr_ = nullptr;
};
