#pragma once

#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include "array_ptr.h"

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
		std::fill(items_.GetRawPtr(), items_.GetRawPtr() + size_, Type{});  // Может бросить исключение
	}

	// Создаёт вектор из size элементов, инициализированных значением value
	SimpleVector(size_t size, const Type& value)
		: items_(size)  // может бросить исключение
		, size_(size)
		, capacity_(size) {
		std::fill(items_.GetRawPtr(), items_.GetRawPtr() + size_, value);  // Может бросить исключение
	}

	// Создаёт вектор из std::initializer_list
	SimpleVector(std::initializer_list<Type> init)
		: items_(init.size())  // Может бросить исключение
		, size_(init.size())
		, capacity_(init.size()) {
		std::copy(init.begin(), init.end(), items_.GetRawPtr());
	}

	// Возвращает количество элементов в массиве
	size_t GetSize() const noexcept {
		return size_;
	}

	// Возвращает вместимость массива
	size_t GetCapacity() const noexcept {
		return capacity_;
	}

	// Сообщает, пустой ли массив
	bool IsEmpty() const noexcept {
		return size_ == 0;
	}

	// Возвращает ссылку на элемент с индексом index
	Type& operator[](size_t index) noexcept {
		assert(index < size_);
		return items_[index];
	}

	// Возвращает константную ссылку на элемент с индексом index
	const Type& operator[](size_t index) const noexcept {
		assert(index < size_);
		return items_[index];
	}

	// Возвращает константную ссылку на элемент с индексом index
	// Выбрасывает исключение std::out_of_range, если index >= size
	Type& At(size_t index) {
		if (index >= size_) {
			throw std::out_of_range("Index out of range");
		}
		return items_[index];
	}

	// Возвращает константную ссылку на элемент с индексом index
	// Выбрасывает исключение std::out_of_range, если index >= size
	const Type& At(size_t index) const {
		if (index >= size_) {
			throw std::out_of_range("Index out of range");
		}
		return items_[index];
	}

	// Обнуляет размер массива, не изменяя его вместимость
	void Clear() noexcept {
		size_ = 0;
	}

	// Изменяет размер массива.
	// При увеличении размера новые элементы получают значение по умолчанию для типа Type
	// TODO Resize
	void Resize(size_t new_size) {
		
		//if (new_size <= size_) {
		//	size_ = new_size;
		//}
		//else if (new_size <= capacity_) {
		//	for (size_t i = size_; i < new_size; ++i) {
		//		*(items_ + i) = Type{};
		//	}
		//	size_ = new_size;
		//}
		//else {
		//	Type* temp = new Type[new_size];
		//	std::fill(temp + size_, temp + new_size, Type{});
		//	for (size_t i = 0; i < size_; ++i) {
		//		(temp + i) = (items_ + i);
		//	}
		//	/*for (size_t i = size_;i < new_size;++i) {
		//		*(temp + i) = Type{};
		//	}*/
		//	delete[] items_;
		//	items_ = temp;
		//	capacity_ = new_size;
		//	size_ = new_size;
		//}

		if (new_size > capacity_) {
			// Вычисляем новую вместимость вектора
			const size_t new_capacity = std::max(capacity_ * 2, new_size);

			// Копируем существующие элементы вектора на новое место
			auto new_items =  ReallocateCopy(new_capacity);  // может выбросить исключение
			// Заполняем добавленные элементы значением по умолчанию
			std::fill(new_items.Get() + size_, new_items.Get() + new_size, Type{});  // может выбросить исключение

			items_.swap(new_items);
			capacity_ = new_capacity;
		}
		else if (new_size > size_) {
			assert(new_size <= capacity_);
			std::fill(items_.GetRawPtr() + size_, items_.GetRawPtr() + new_size, Type{});  // может выбросить исключение
		}

		// Во всех случаях устанавливается новый размер
		size_ = new_size;
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
	ArrayPtr<Type> items_;

	size_t size_ = 0;
	size_t capacity_ = 0;
};
