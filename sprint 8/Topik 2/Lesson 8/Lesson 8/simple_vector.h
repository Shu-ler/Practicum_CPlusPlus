/*
|	Филимонов И.В.
|		+7 913 240 81 77
|		+7 905 084 20 06 (Telegram)
|		https://github.com/Shu-ler
*/

#pragma once

#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <compare>
#include "array_ptr.h"

// Структура хранения потребной вместимости с конструктором
// Используется для передачи в конструктор с зарезервированной емкостью
struct ReserveProxyObj {
	explicit ReserveProxyObj(size_t capacity_to_reserve) : capacity(capacity_to_reserve) {
	}
	size_t capacity;
};

// Возвращает прокси-объект для конструктора
ReserveProxyObj Reserve(size_t capacity_to_reserve) {
	return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {
public:
	using Iterator = Type*;
	using ConstIterator = const Type*;

	SimpleVector() noexcept = default;

	// Конструктор вектора из size элементов, инициализированных значением по умолчанию
	explicit SimpleVector(size_t size)
		: items_(size)
		, size_(size)
		, capacity_(size) {
		std::generate(begin(), end(), []{ 
			return Type{}; 
			});
	}

	// Конструктор вектора из size элементов, инициализированных значением value
	SimpleVector(size_t size, const Type& value)
		: items_(size)  
		, size_(size)
		, capacity_(size) {
		std::fill(begin(), end(), value);
	}

	// Конструктор вектора из std::initializer_list
	SimpleVector(std::initializer_list<Type> init)
		: items_(init.size())  // Может бросить исключение
		, size_(init.size())
		, capacity_(init.size()) {
		std::copy(init.begin(), init.end(), this->begin());
	}

	// Конструктор вектора с зарезервированной памятью
	// Пример использования:
	//		SimpleVector<int> v(Reserve(77));
	SimpleVector(ReserveProxyObj reserved)
		: items_(reserved.capacity)
		, size_(0)
		, capacity_(reserved.capacity) {
	}

	// Конструктор копирования
	SimpleVector(const SimpleVector& other)
		: items_(other.size_)
		, size_(other.size_)
		, capacity_(other.size_) {
		std::copy(other.begin(), other.end(), this->begin());
	}

	// Move-конструктор
	SimpleVector(SimpleVector&& other)
		: items_(std::move(other.items_))
		, size_(std::exchange(other.size_, 0))
		, capacity_(std::exchange(other.capacity_, 0)) { 
		}

	// Оператор присваивания
	SimpleVector& operator=(const SimpleVector& other) {
		if (&other != this) {				// Проверка на самоприсваивание
			SimpleVector other_copy(other);	// Создаём временную копию rhs
			swap(other_copy);				// Меняем содержимое текущего объекта с помощью swap
		}
		return *this;
	}

	// Move-оператора присваивания
	SimpleVector& operator=(SimpleVector&& other) {
		items_ = std::move(other.items_);
		size_ = std::exchange(other.size_, 0);
		capacity_ = std::exchange(other.capacity_, 0);
		return *this;
	}

	// Добавляет элемент в конец вектора
	void PushBack(const Type& item) {
		if (IsFull()) {
			IncCapacity();
		}
		items_[size_++] = item;
	}

	// Добавляет элемент в конец вектора
	void PushBack(Type&& item) {
		if (IsFull()) {
			IncCapacity();
		}
		items_[size_++] = std::move(item);
	}

	// Вставляет значение value в позицию pos.
	// Возвращает итератор на вставленное значение
	Iterator Insert(ConstIterator pos, const Type& value) {
		assert(begin() <= pos && pos <= end());			// Проверка попадания в диапазон
		size_t offset = std::distance(cbegin(), pos);	// Вычисление индекса позиции для вставки

		if (IsFull()) {									// Проверка вместимости
			IncCapacity();								// Увеличение емкости. Итераторы инвалидируются.
		}

		++size_;
		Iterator  it = begin() + offset;				// Итератор на позицию вставки
		std::copy_backward(it, end() - 1, end());		// Сдвиг элементов вправо начиная с позиции вставки
		*it = value;									// Установка значения вставленного элемента
		return it;										// Возврат итератора на вставленный элемент
	}

	// Вставляет значение value в позицию pos.
	// Возвращает итератор на вставленное значение
	Iterator Insert(ConstIterator pos, Type&& value) {
		assert(begin() <= pos && pos <= end());			// Проверка попадания в диапазон
		size_t offset = std::distance(cbegin(), pos);	// Вычисление индекса позиции для вставки

		if (IsFull()) {									// Проверка вместимости
			IncCapacity();								// Увеличение емкости. Итераторы инвалидируются.
		}

		++size_;
		Iterator  it = begin() + offset;				// Итератор на позицию вставки
		std::copy_backward(std::make_move_iterator(it), std::make_move_iterator(end() - 1), end());
		*it = std::move(value);
		return it;										// Возврат итератора на вставленный элемент
	}

	// "Удаляет" последний элемент вектора. Вектор не должен быть пустым
	void PopBack() noexcept {
		if (!IsEmpty()) {
			--size_;
		}
	}

	// Удаляет элемент вектора в указанной позиции
	Iterator Erase(ConstIterator pos) {
		assert(begin() <= pos && pos < end()); // Проверка попадания в диапазон
		Iterator erase_pos = const_cast<Iterator>(pos);
		std::move(erase_pos + 1, end(), erase_pos);
		--size_;
		return erase_pos;
	}

	// Обменивает значение с другим вектором
	void swap(SimpleVector& other) noexcept {
		items_.swap(other.items_);
		std::swap(size_, other.size_);
		std::swap(capacity_, other.capacity_);
	}

	// Возвращает количество элементов в векторе
	size_t GetSize() const noexcept {
		return size_;
	}

	// Возвращает вместимость вектора
	size_t GetCapacity() const noexcept {
		return capacity_;
	}

	// Сообщает, пустой ли вектор
	bool IsEmpty() const noexcept {
		return size_ == 0;
	}

	// Сообщает, заполнен ли вектор полностью
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

	// Возвращает ссылку на элемент с индексом index
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

	// Изменяет размер вектор.
	// При увеличении размера новые элементы получают значение по умолчанию для типа Type
	void Resize(size_t new_size) {
		if (new_size > capacity_) {
			const size_t new_capacity = NewCapacity(new_size);	// Вычисляем новую вместимость вектора
			SimpleVector<Type> buffer(new_capacity);			// Создаем новый вектор
			std::move(begin(), end(), buffer.begin());			// Копируем содержимое старого
			swap(buffer);
		}
		else if (new_size > size_) {
			assert(new_size <= capacity_);
			std::generate(end(), begin() + new_size, [] {
				return Type{};
				});
		}
		size_ = new_size;	// Во всех случаях устанавливается новый размер
	}

	// Изменяет зарезервированную ёмкость вектора
	void Reserve(size_t new_capacity) {
		if (new_capacity > capacity_) {
			size_t size_save = size_;
			Resize(new_capacity);
			size_ = size_save;
			capacity_ = new_capacity;
		}
	};

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
	// Увеличивает емкость вектора
	void IncCapacity() {
		SimpleVector<Type> buffer(NewCapacity());	// Создаём новый вектор с увеличенной вместимостью
		buffer.size_ = size_;						// Устанавливаем правильно size_
		std::move(begin(), end(), buffer.begin());	// Перемещаем элементы из текущего вектора в буфер
		swap(buffer);								// Меняем содержимое текущего вектора с буфером
	}

	// Возвращает новое значение вместимости
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
