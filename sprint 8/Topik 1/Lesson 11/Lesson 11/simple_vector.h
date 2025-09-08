#pragma once

/*
|	��������� �.�.
|		+7 913 240 81 77
|		+7 905 084 20 06 (Telegram)
|		https://github.com/Shu-ler
*/		

// ���������� ���������� SimpleVector
// ����� ������

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

	// ������ ������ �� size ���������, ������������������ ��������� �� ���������
	explicit SimpleVector(size_t size)
		: items_(size)  // ����� ������� ����������
		, size_(size)
		, capacity_(size) {
		std::fill(items_.GetRawPtr(), items_.GetRawPtr() + size_, Type{});  // ����� ������� ����������
	}

	// ������ ������ �� size ���������, ������������������ ��������� value
	SimpleVector(size_t size, const Type& value)
		: items_(size)  // ����� ������� ����������
		, size_(size)
		, capacity_(size) {
		std::fill(items_.GetRawPtr(), items_.GetRawPtr() + size_, value);  // ����� ������� ����������
	}

	// ������ ������ �� std::initializer_list
	SimpleVector(std::initializer_list<Type> init)
		: items_(init.size())  // ����� ������� ����������
		, size_(init.size())
		, capacity_(init.size()) {
		std::copy(init.begin(), init.end(), items_.GetRawPtr());
	}
	// ����������� �����������
	SimpleVector(const SimpleVector& other) {
		// TODO: ����������� �����������
	}

	// �������� ������������
	SimpleVector& operator=(const SimpleVector& rhs) {
		// TODO: �������� ������������
		return *this;
	}

	// ��������� ������� � ����� �������
	// ��� �������� ����� ����������� ����� ����������� �������
	void PushBack(const Type& item) {
		// TODO: ����� PushBack
	}

	// ��������� �������� value � ������� pos.
	// ���������� �������� �� ����������� ��������
	// ���� ����� �������� �������� ������ ��� �������� ���������,
	// ����������� ������� ������ ����������� �����, � ��� ������� ������������ 0 ����� ������ 1
	Iterator Insert(ConstIterator pos, const Type& value) {
		// TODO: ����� Insert
	}

	// "�������" ��������� ������� �������. ������ �� ������ ���� ������
	void PopBack() noexcept {
		// TODO: ����� PopBack
	}

	// ������� ������� ������� � ��������� �������
	Iterator Erase(ConstIterator pos) {
		// TODO: ����� Erase
	}

	// ���������� �������� � ������ ��������
	void swap(SimpleVector& other) noexcept {
		// TODO: ����� swap
	}

	// ���������� ���������� ��������� � �������
	size_t GetSize() const noexcept {
		return size_;
	}

	// ���������� ����������� �������
	size_t GetCapacity() const noexcept {
		return capacity_;
	}

	// ��������, ������ �� ������
	bool IsEmpty() const noexcept {
		return size_ == 0;
	}

	// ���������� ������ �� ������� � �������� index
	Type& operator[](size_t index) noexcept {
		assert(index < size_);
		return items_.GetRawPtr()[index];
	}

	// ���������� ����������� ������ �� ������� � �������� index
	const Type& operator[](size_t index) const noexcept {
		assert(index < size_);
		return items_.GetRawPtr()[index];
	}

	// ���������� ����������� ������ �� ������� � �������� index
	// ����������� ���������� std::out_of_range, ���� index >= size
	Type& At(size_t index) {
		if (index >= size_) {
			throw std::out_of_range("Index out of range");
		}
		return items_.GetRawPtr()[index];
	}

	// ���������� ����������� ������ �� ������� � �������� index
	// ����������� ���������� std::out_of_range, ���� index >= size
	const Type& At(size_t index) const {
		if (index >= size_) {
			throw std::out_of_range("Index out of range");
		}
		return items_.GetRawPtr()[index];
	}

	// �������� ������ �������, �� ������� ��� �����������
	void Clear() noexcept {
		size_ = 0;
	}

	// �������� ������ �������.
	// ��� ���������� ������� ����� �������� �������� �������� �� ��������� ��� ���� Type
	// TODO Resize
	void Resize(size_t new_size) {

		if (new_size > capacity_) {
			// ��������� ����� ����������� �������
			const size_t new_capacity = std::max(capacity_ * 2, new_size);

			// �������� ������������ �������� ������� �� ����� �����
			auto new_items = ReallocateCopy(new_capacity);  // ����� ������� ����������
			// ��������� ����������� �������� ��������� �� ���������
			std::fill(new_items.GetRawPtr() + size_, new_items.GetRawPtr() + new_size, Type{});  // ����� ������� ����������

			items_.swap(new_items);
			capacity_ = new_capacity;
		}
		else if (new_size > size_) {
			assert(new_size <= capacity_);
			std::fill(items_.GetRawPtr() + size_, items_.GetRawPtr() + new_size, Type{});  // ����� ������� ����������
		}

		// �� ���� ������� ��������������� ����� ������
		size_ = new_size;
	}

	// ���������� �������� �� ������ �������
	// ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
	Iterator begin() noexcept {
		return items_.GetRawPtr();
	}

	// ���������� �������� �� �������, ��������� �� ���������
	// ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
	Iterator end() noexcept {
		return items_.GetRawPtr() + size_;
	}

	// ���������� ����������� �������� �� ������ �������
	// ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
	ConstIterator begin() const noexcept {
		return items_.GetRawPtr();
	}

	// ���������� �������� �� �������, ��������� �� ���������
	// ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
	ConstIterator end() const noexcept {
		return items_.GetRawPtr() + size_;
	}

	// ���������� ����������� �������� �� ������ �������
	// ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
	ConstIterator cbegin() const noexcept {
		return items_.GetRawPtr();
	}

	// ���������� �������� �� �������, ��������� �� ���������
	// ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
	ConstIterator cend() const noexcept {
		return items_.GetRawPtr() + size_;
	}

private:
	// ��������������� ����� ��� ��������� ����� �������� ������� � �������� ������������
	ArrayPtr<Type> ReallocateCopy(size_t new_capacity) const {
		ArrayPtr<Type> new_items(new_capacity);  // ����� ������� ����������
		size_t copy_size = std::min(new_capacity, size_);
		std::copy(items_.GetRawPtr(), items_.GetRawPtr() + copy_size, new_items.GetRawPtr());  // ����� ������� ����������
		return new_items;
	}

private:
	ArrayPtr<Type> items_;

	size_t size_ = 0;
	size_t capacity_ = 0;
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	// TODO: �������� ==
	return true;
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	// TODO: �������� !=
	return true;
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	// TODO: �������� <
	return true;
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	// TODO: �������� <=
	return true;
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	// TODO: �������� >
	return true;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	// TODO: �������� >=
	return true;
}
