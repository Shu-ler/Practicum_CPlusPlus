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
#include <compare>
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
	SimpleVector(const SimpleVector& other)
		: items_(other.size_)  // выделяем память для копирования элементов
		, size_(other.size_)
		, capacity_(other.size_) {
		std::copy(other.items_.GetRawPtr(), other.items_.GetRawPtr() + size_, items_.GetRawPtr());
	}

	~SimpleVector() {
		auto to_del = items_.Release();
		delete[] to_del;
	}

	// Оператор присваивания
	SimpleVector& operator=(const SimpleVector& rhs) {
		if (&rhs != this) {				// Проверка на самоприсваивание
			SimpleVector rhs_copy(rhs);	// Создаём временную копию rhs
			swap(rhs_copy);				// Меняем содержимое текущего объекта с помощью swap
		}
		return *this;
	}

	// Добавляет элемент в конец вектора
	// При нехватке места увеличивает вдвое вместимость вектора
	void PushBack(const Type& item) {
		if (IsFull()) {
			IncCapacity();
		}
		items_[size_++] = item;
	}

	// Вставляет значение value в позицию pos.
	// Возвращает итератор на вставленное значение
	// Если перед вставкой значения вектор был заполнен полностью,
	// вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
	Iterator Insert(ConstIterator pos, const Type& value) {
		assert(begin() <= pos);
		assert(pos <= end());

		// Вычисляем индекс позиции для вставки
		size_t offset = std::distance(cbegin(), pos);

		// Проверяем, достаточно ли вместимости
		if (IsFull()) {
			IncCapacity();
		}

		++size_;
		Iterator  it = begin() + offset;
		// Сдвигаем элементы вправо начиная с позиции вставки
		std::copy_backward(it, end() - 1, end());

		// Вставляем новый элемент
		*it = value;

		return it;  // Возвращаем итератор на вставленный элемент
	}

	// "Удаляет" последний элемент вектора. Вектор не должен быть пустым
	void PopBack() noexcept {
		if (!IsEmpty()) {
			--size_;
		}
	}

	// Удаляет элемент вектора в указанной позиции
	Iterator Erase(ConstIterator pos) {
		assert(begin() <= pos);
		assert(pos <= end());
		Iterator erase_pos = const_cast<Iterator>(pos);
		if (size_ != 0) {
			std::copy(erase_pos + 1, end(), erase_pos);
			--size_;
		}
		return erase_pos;
	}

	// Обменивает значение с другим вектором
	void swap(SimpleVector& other) noexcept {
		//std::swap(items_, other.items_);
		items_.swap(other.items_);
		std::swap(size_, other.size_);
		std::swap(capacity_, other.capacity_);
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

	// Сообщает, заполнен ли массив полностью
	bool IsFull() const noexcept {
		return size_ == capacity_;
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

		if (new_size > capacity_) {
			const size_t new_capacity = NewCapacity(new_size);	// Вычисляем новую вместимость вектора
			SimpleVector<Type> buffer(new_capacity);			// Создаем новый вектор
			std::copy(begin(), end(), buffer.begin());			// Копируем содержимое старого
			swap(buffer);
		}
		else if (new_size > size_) {
			assert(new_size <= capacity_);
			std::fill(end(), begin() + new_size, Type{});  // может бросить исключение
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
	void IncCapacity() {
		SimpleVector<Type> buffer(NewCapacity());	// Создаём новый вектор с увеличенной ёмкостью
		buffer.size_ = size_;						// Устанавливаем правильно size_
		std::copy(begin(), end(), buffer.begin());	// Копируем элементы из текущего вектора в буфер
		swap(buffer);								// Меняем содержимое текущего вектора с буфером
	}

	size_t NewCapacity(const size_t new_size = 1) {
		return std::max(capacity_ * 2, new_size);
	}

private:
	ArrayPtr<Type> items_;
	size_t size_ = 0;
	size_t capacity_ = 0;
};

template <typename Type>
inline std::strong_ordering operator<=>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	return std::lexicographical_compare_three_way(
		lhs.cbegin(), lhs.cend(),
		rhs.cbegin(), rhs.cend());
}

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	return (lhs <=> rhs) == std::strong_ordering::equal;
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	return (lhs <=> rhs) != std::strong_ordering::equal;
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	return (lhs <=> rhs) == std::strong_ordering::less;
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	return (lhs <=> rhs) != std::strong_ordering::greater;
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	return (lhs <=> rhs) == std::strong_ordering::greater;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	return (lhs <=> rhs) != std::strong_ordering::less;
}
