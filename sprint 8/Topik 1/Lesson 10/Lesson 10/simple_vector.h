#pragma once

#include <cassert>
#include <initializer_list>

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
		std::fill(items_.Get(), items_.Get() + size_, Type{});  // ����� ������� ����������
	}

	// ������ ������ �� size ���������, ������������������ ��������� value
	SimpleVector(size_t size, const Type& value)
		: items_(size)  // ����� ������� ����������
		, size_(size)
		, capacity_(size) {
		std::fill(items_.Get(), items_.Get() + size_, value);  // ����� ������� ����������
	}

	// ������ ������ �� std::initializer_list
	SimpleVector(std::initializer_list<Type> init)
		: items_(init.size())  // ����� ������� ����������
		, size_(init.size())
		, capacity_(init.size()) {
		std::copy(init.begin(), init.end(), items_.Get());
	}

	// ���������� ���������� ��������� � �������
	size_t GetSize() const noexcept {
		// TODO: ���� ��������������
		return 0;
	}

	// ���������� ����������� �������
	size_t GetCapacity() const noexcept {
		// TODO: ���� ��������������
		return 0;
	}

	// ��������, ������ �� ������
	bool IsEmpty() const noexcept {
		return true;
	}

	// ���������� ������ �� ������� � �������� index
	Type& operator[](size_t index) noexcept {
		// TODO: ���� ��������������
	}

	// ���������� ����������� ������ �� ������� � �������� index
	const Type& operator[](size_t index) const noexcept {
		// TODO: ���� ��������������
	}

	// ���������� ����������� ������ �� ������� � �������� index
	// ����������� ���������� std::out_of_range, ���� index >= size
	Type& At(size_t index) {
		// TODO: ���� ��������������
	}

	// ���������� ����������� ������ �� ������� � �������� index
	// ����������� ���������� std::out_of_range, ���� index >= size
	const Type& At(size_t index) const {
		// TODO: ���� ��������������
	}

	// �������� ������ �������, �� ������� ��� �����������
	void Clear() noexcept {
		// TODO: ���� ��������������
	}

	// �������� ������ �������.
	// ��� ���������� ������� ����� �������� �������� �������� �� ��������� ��� ���� Type
	void Resize(size_t new_size) {
		// TODO: ���� ��������������
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
	// ������ ������ ��������� ����� ������������ ����� ���������, ����� ��� ArrayPtr
	Type* items_{};

	size_t size_ = 0;
	size_t capacity_ = 0;
};
