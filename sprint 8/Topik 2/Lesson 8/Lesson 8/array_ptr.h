/*
|	Филимонов И.В.
|		+7 913 240 81 77
|		+7 905 084 20 06 (Telegram)
|		https://github.com/Shu-ler
*/
#pragma once

#include <stdexcept>
#include <utility>

// Умный указатель, удаляющий связанный объект при своём разрушении.
// Параметр шаблона T задаёт тип объекта, на который ссылается указатель
template <typename T>
class ArrayPtr {
public:
	// Конструктор по умолчанию 
	// Создаёт нулевой указатель
	ArrayPtr() = default;

	// Конструктор на базе сырого указателя
	explicit ArrayPtr(T* raw_ptr) noexcept : ptr_(raw_ptr) {
	}

	// Конструктор объекта заданного размера
	explicit ArrayPtr(size_t size)
		: ptr_(size > 0 ? new T[size] : nullptr) {
	}

	// Move - конструктор 
	// Создает новый объект путем перемещения ресурсов другого объекта
	ArrayPtr(ArrayPtr&& other) noexcept : ptr_(std::exchange(other.ptr_, nullptr)) {
	}

	// Удаляем у класса конструктор копирования
//	ArrayPtr(const ArrayPtr&) = delete;

	// Деструктор. Удаляет объект, на который ссылается умный указатель.
	~ArrayPtr() {
		delete[] ptr_;
	}

	// Оператор приведения к типу bool позволяет узнать, ссылается ли умный указатель
	// на какой-либо объект
	explicit operator bool() const noexcept {
		return ptr_ != nullptr;
	}

	// Move-оператор присваивания
	ArrayPtr& operator=(ArrayPtr&& other) noexcept {
		if (this != &other) {
			delete[] ptr_;
			ptr_ = std::exchange(other.ptr_, nullptr);
		}
		return *this;
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

	// Оператор индексирования (доступ к элементу массива по индексу)
	T& operator[](size_t index) noexcept {
		return ptr_[index];
	}

	// Константный оператор индексирования (доступ к элементу массива по индексу)
	const T& operator[](size_t index) const noexcept {
		return ptr_[index];
	}

	// Возвращает сырой указатель
	T* GetRawPtr() noexcept {
		return ptr_;
	}

	// Возвращает сырой указатель
	const T* GetRawPtr() const noexcept {
		return ptr_;
	}

	// Прекращает владение объектом, на который ссылается умный указатель.
	// Возвращает прежнее значение "сырого" указателя и устанавливает поле ptr_ в null
	T* Release() noexcept {
		T* tmp = ptr_;
		ptr_ = nullptr;
		return tmp;
	}

	// Обмен значениями двух объектов 
	void swap(ArrayPtr& other) noexcept {
		std::swap(ptr_, other.ptr_);
	}

private:
	T* ptr_ = nullptr;
};
