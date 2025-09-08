#pragma once

/*
|	Филимонов И.В.
|		+7 913 240 81 77
|		+7 905 084 20 06 (Telegram)
|		https://github.com/Shu-ler
*/		

// Разработка контейнера SimpleVector
// Часть вторая

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
	// Конструктор копирования
	SimpleVector(const SimpleVector& other) {
		// TODO: конструктор копирования
	}

	// Оператор присваивания
	SimpleVector& operator=(const SimpleVector& rhs) {
		// TODO: оператор присваивания
		return *this;
	}

	// Добавляет элемент в конец вектора
	// При нехватке места увеличивает вдвое вместимость вектора
	void PushBack(const Type& item) {
		// TODO: метод PushBack
	}

	// Вставляет значение value в позицию pos.
	// Возвращает итератор на вставленное значение
	// Если перед вставкой значения вектор был заполнен полностью,
	// вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
	Iterator Insert(ConstIterator pos, const Type& value) {
		// TODO: метод Insert
	}

	// "Удаляет" последний элемент вектора. Вектор не должен быть пустым
	void PopBack() noexcept {
		// TODO: метод PopBack
	}

	// Удаляет элемент вектора в указанной позиции
	Iterator Erase(ConstIterator pos) {
		// TODO: метод Erase
	}

	// Обменивает значение с другим вектором
	void swap(SimpleVector& other) noexcept {
		// TODO: метод swap
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
		return items_.GetRawPtr()[index];
	}

	// Возвращает константную ссылку на элемент с индексом index
	const Type& operator[](size_t index) const noexcept {
		assert(index < size_);
		return items_.GetRawPtr()[index];
	}

	// Возвращает константную ссылку на элемент с индексом index
	// Выбрасывает исключение std::out_of_range, если index >= size
	Type& At(size_t index) {
		if (index >= size_) {
			throw std::out_of_range("Index out of range");
		}
		return items_.GetRawPtr()[index];
	}

	// Возвращает константную ссылку на элемент с индексом index
	// Выбрасывает исключение std::out_of_range, если index >= size
	const Type& At(size_t index) const {
		if (index >= size_) {
			throw std::out_of_range("Index out of range");
		}
		return items_.GetRawPtr()[index];
	}

	// Обнуляет размер массива, не изменяя его вместимость
	void Clear() noexcept {
		size_ = 0;
	}

	// Изменяет размер массива.
	// При увеличении размера новые элементы получают значение по умолчанию для типа Type
	// TODO Resize
	void Resize(size_t new_size) {

		if (new_size > capacity_) {
			// Вычисляем новую вместимость вектора
			const size_t new_capacity = std::max(capacity_ * 2, new_size);

			// Копируем существующие элементы вектора на новое место
			auto new_items = ReallocateCopy(new_capacity);  // может бросить исключение
			// Заполняем добавленные элементы значением по умолчанию
			std::fill(new_items.GetRawPtr() + size_, new_items.GetRawPtr() + new_size, Type{});  // может бросить исключение

			items_.swap(new_items);
			capacity_ = new_capacity;
		}
		else if (new_size > size_) {
			assert(new_size <= capacity_);
			std::fill(items_.GetRawPtr() + size_, items_.GetRawPtr() + new_size, Type{});  // может бросить исключение
		}

		// Во всех случаях устанавливается новый размер
		size_ = new_size;
	}

	// Возвращает итератор на начало массива
	// Для пустого массива может быть равен (или не равен) nullptr
	Iterator begin() noexcept {
		return items_.GetRawPtr();
	}

	// Возвращает итератор на элемент, следующий за последним
	// Для пустого массива может быть равен (или не равен) nullptr
	Iterator end() noexcept {
		return items_.GetRawPtr() + size_;
	}

	// Возвращает константный итератор на начало массива
	// Для пустого массива может быть равен (или не равен) nullptr
	ConstIterator begin() const noexcept {
		return items_.GetRawPtr();
	}

	// Возвращает итератор на элемент, следующий за последним
	// Для пустого массива может быть равен (или не равен) nullptr
	ConstIterator end() const noexcept {
		return items_.GetRawPtr() + size_;
	}

	// Возвращает константный итератор на начало массива
	// Для пустого массива может быть равен (или не равен) nullptr
	ConstIterator cbegin() const noexcept {
		return items_.GetRawPtr();
	}

	// Возвращает итератор на элемент, следующий за последним
	// Для пустого массива может быть равен (или не равен) nullptr
	ConstIterator cend() const noexcept {
		return items_.GetRawPtr() + size_;
	}

private:
	// Вспомогательный метод для выделения копии текущего массива с заданной вместимостью
	ArrayPtr<Type> ReallocateCopy(size_t new_capacity) const {
		ArrayPtr<Type> new_items(new_capacity);  // может бросить исключение
		size_t copy_size = std::min(new_capacity, size_);
		std::copy(items_.GetRawPtr(), items_.GetRawPtr() + copy_size, new_items.GetRawPtr());  // может бросить исключение
		return new_items;
	}

private:
	ArrayPtr<Type> items_;

	size_t size_ = 0;
	size_t capacity_ = 0;
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	// TODO: оператор ==
	return true;
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	// TODO: оператор !=
	return true;
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	// TODO: оператор <
	return true;
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	// TODO: оператор <=
	return true;
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	// TODO: оператор >
	return true;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	// TODO: оператор >=
	return true;
}
