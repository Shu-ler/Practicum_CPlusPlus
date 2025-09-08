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
#include <compare>
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
	SimpleVector(const SimpleVector& other)
		: items_(other.size_)  // �������� ������ ��� ����������� ���������
		, size_(other.size_)
		, capacity_(other.size_) {
		std::copy(other.items_.GetRawPtr(), other.items_.GetRawPtr() + size_, items_.GetRawPtr());
	}

	~SimpleVector() {
		auto to_del = items_.Release();
		delete[] to_del;
	}

	// �������� ������������
	SimpleVector& operator=(const SimpleVector& rhs) {
		if (&rhs != this) {				// �������� �� ����������������
			SimpleVector rhs_copy(rhs);	// ������ ��������� ����� rhs
			swap(rhs_copy);				// ������ ���������� �������� ������� � ������� swap
		}
		return *this;
	}

	// ��������� ������� � ����� �������
	// ��� �������� ����� ����������� ����� ����������� �������
	void PushBack(const Type& item) {
		if (IsFull()) {
			IncCapacity();
		}
		items_[size_++] = item;
	}

	// ��������� �������� value � ������� pos.
	// ���������� �������� �� ����������� ��������
	// ���� ����� �������� �������� ������ ��� �������� ���������,
	// ����������� ������� ������ ����������� �����, � ��� ������� ������������ 0 ����� ������ 1
	Iterator Insert(ConstIterator pos, const Type& value) {
		assert(begin() <= pos);
		assert(pos <= end());

		// ��������� ������ ������� ��� �������
		size_t offset = std::distance(cbegin(), pos);

		// ���������, ���������� �� �����������
		if (IsFull()) {
			IncCapacity();
		}

		++size_;
		Iterator  it = begin() + offset;
		// �������� �������� ������ ������� � ������� �������
		std::copy_backward(it, end() - 1, end());

		// ��������� ����� �������
		*it = value;

		return it;  // ���������� �������� �� ����������� �������
	}

	// "�������" ��������� ������� �������. ������ �� ������ ���� ������
	void PopBack() noexcept {
		if (!IsEmpty()) {
			--size_;
		}
	}

	// ������� ������� ������� � ��������� �������
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

	// ���������� �������� � ������ ��������
	void swap(SimpleVector& other) noexcept {
		//std::swap(items_, other.items_);
		items_.swap(other.items_);
		std::swap(size_, other.size_);
		std::swap(capacity_, other.capacity_);
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

	// ��������, �������� �� ������ ���������
	bool IsFull() const noexcept {
		return size_ == capacity_;
	}

	// ���������� ������ �� ������� � �������� index
	Type& operator[](size_t index) noexcept {
		assert(index < size_);
		return items_[index];
	}

	// ���������� ����������� ������ �� ������� � �������� index
	const Type& operator[](size_t index) const noexcept {
		assert(index < size_);
		return items_[index];
	}

	// ���������� ����������� ������ �� ������� � �������� index
	// ����������� ���������� std::out_of_range, ���� index >= size
	Type& At(size_t index) {
		if (index >= size_) {
			throw std::out_of_range("Index out of range");
		}
		return items_[index];
	}

	// ���������� ����������� ������ �� ������� � �������� index
	// ����������� ���������� std::out_of_range, ���� index >= size
	const Type& At(size_t index) const {
		if (index >= size_) {
			throw std::out_of_range("Index out of range");
		}
		return items_[index];
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
			const size_t new_capacity = NewCapacity(new_size);	// ��������� ����� ����������� �������
			SimpleVector<Type> buffer(new_capacity);			// ������� ����� ������
			std::copy(begin(), end(), buffer.begin());			// �������� ���������� �������
			swap(buffer);
		}
		else if (new_size > size_) {
			assert(new_size <= capacity_);
			std::fill(end(), begin() + new_size, Type{});  // ����� ������� ����������
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
	void IncCapacity() {
		SimpleVector<Type> buffer(NewCapacity());	// ������ ����� ������ � ����������� ��������
		buffer.size_ = size_;						// ������������� ��������� size_
		std::copy(begin(), end(), buffer.begin());	// �������� �������� �� �������� ������� � �����
		swap(buffer);								// ������ ���������� �������� ������� � �������
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
