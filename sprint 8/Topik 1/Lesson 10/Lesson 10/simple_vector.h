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
			// ��������� ����� ����������� �������
			const size_t new_capacity = std::max(capacity_ * 2, new_size);

			// �������� ������������ �������� ������� �� ����� �����
			auto new_items =  ReallocateCopy(new_capacity);  // ����� ��������� ����������
			// ��������� ����������� �������� ��������� �� ���������
			std::fill(new_items.Get() + size_, new_items.Get() + new_size, Type{});  // ����� ��������� ����������

			items_.swap(new_items);
			capacity_ = new_capacity;
		}
		else if (new_size > size_) {
			assert(new_size <= capacity_);
			std::fill(items_.GetRawPtr() + size_, items_.GetRawPtr() + new_size, Type{});  // ����� ��������� ����������
		}

		// �� ���� ������� ��������������� ����� ������
		size_ = new_size;
	}

	// ���������� �������� �� ������ �������
	// ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
	Iterator begin() noexcept {
		// TODO: ���� ��������������
	}

	// ���������� �������� �� �������, ��������� �� ���������
	// ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
	Iterator end() noexcept {
		// TODO: ���� ��������������
	}

	// ���������� ����������� �������� �� ������ �������
	// ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
	ConstIterator begin() const noexcept {
		// TODO: ���� ��������������
	}

	// ���������� �������� �� �������, ��������� �� ���������
	// ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
	ConstIterator end() const noexcept {
		// TODO: ���� ��������������
	}

	// ���������� ����������� �������� �� ������ �������
	// ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
	ConstIterator cbegin() const noexcept {
		// TODO: ���� ��������������
	}

	// ���������� �������� �� �������, ��������� �� ���������
	// ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
	ConstIterator cend() const noexcept {
		// TODO: ���� ��������������
	}

private:
	ArrayPtr<Type> items_;

	size_t size_ = 0;
	size_t capacity_ = 0;
};
