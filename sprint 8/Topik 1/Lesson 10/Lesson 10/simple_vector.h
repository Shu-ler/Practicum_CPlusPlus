#pragma once

#include <cassert>
#include <initializer_list>

template <typename Type>
class SimpleVector {
public:
	using Iterator = Type*;
	using ConstIterator = const Type*;

	SimpleVector() noexcept = default;

	// Создаёт вектор из size элементов, инициализированных значением по умолчанию
	explicit SimpleVector(size_t size)
		: items_(size)  // может бросить исключение
		, size_(size)
		, capacity_(size) {
		std::fill(items_.Get(), items_.Get() + size_, Type{});  // Может бросить исключение
	}

	// Создаёт вектор из size элементов, инициализированных значением value
	SimpleVector(size_t size, const Type& value)
		: items_(size)  // может бросить исключение
		, size_(size)
		, capacity_(size) {
		std::fill(items_.Get(), items_.Get() + size_, value);  // Может бросить исключение
	}

	// Создаёт вектор из std::initializer_list
	SimpleVector(std::initializer_list<Type> init)
		: items_(init.size())  // Может бросить исключение
		, size_(init.size())
		, capacity_(init.size()) {
		std::copy(init.begin(), init.end(), items_.Get());
	}

	// Возвращает количество элементов в массиве
	size_t GetSize() const noexcept {
		// TODO: тело самостоятельно
		return 0;
	}

	// Возвращает вместимость массива
	size_t GetCapacity() const noexcept {
		// TODO: тело самостоятельно
		return 0;
	}

	// Сообщает, пустой ли массив
	bool IsEmpty() const noexcept {
		return true;
	}

	// Возвращает ссылку на элемент с индексом index
	Type& operator[](size_t index) noexcept {
		// TODO: тело самостоятельно
	}

	// Возвращает константную ссылку на элемент с индексом index
	const Type& operator[](size_t index) const noexcept {
		// TODO: тело самостоятельно
	}

	// Возвращает константную ссылку на элемент с индексом index
	// Выбрасывает исключение std::out_of_range, если index >= size
	Type& At(size_t index) {
		// TODO: тело самостоятельно
	}

	// Возвращает константную ссылку на элемент с индексом index
	// Выбрасывает исключение std::out_of_range, если index >= size
	const Type& At(size_t index) const {
		// TODO: тело самостоятельно
	}

	// Обнуляет размер массива, не изменяя его вместимость
	void Clear() noexcept {
		// TODO: тело самостоятельно
	}

	// Изменяет размер массива.
	// При увеличении размера новые элементы получают значение по умолчанию для типа Type
	void Resize(size_t new_size) {
		// TODO: тело самостоятельно
	}

	// Возвращает итератор на начало массива
	// Для пустого массива может быть равен (или не равен) nullptr
	Iterator begin() noexcept {
		// TODO: тело самостоятельно
	}

	// Возвращает итератор на элемент, следующий за последним
	// Для пустого массива может быть равен (или не равен) nullptr
	Iterator end() noexcept {
		// TODO: тело самостоятельно
	}

	// Возвращает константный итератор на начало массива
	// Для пустого массива может быть равен (или не равен) nullptr
	ConstIterator begin() const noexcept {
		// TODO: тело самостоятельно
	}

	// Возвращает итератор на элемент, следующий за последним
	// Для пустого массива может быть равен (или не равен) nullptr
	ConstIterator end() const noexcept {
		// TODO: тело самостоятельно
	}

	// Возвращает константный итератор на начало массива
	// Для пустого массива может быть равен (или не равен) nullptr
	ConstIterator cbegin() const noexcept {
		// TODO: тело самостоятельно
	}

	// Возвращает итератор на элемент, следующий за последним
	// Для пустого массива может быть равен (или не равен) nullptr
	ConstIterator cend() const noexcept {
		// TODO: тело самостоятельно
	}

private:
	// Вместо сырого указателя лучше использовать умный указатель, такой как ArrayPtr
	Type* items_{};

	size_t size_ = 0;
	size_t capacity_ = 0;
};
