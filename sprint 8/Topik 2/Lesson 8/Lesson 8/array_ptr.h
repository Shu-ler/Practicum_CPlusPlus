/*
|	��������� �.�.
|		+7 913 240 81 77
|		+7 905 084 20 06 (Telegram)
|		https://github.com/Shu-ler
*/
#pragma once

#include <stdexcept>
#include <utility>

// ����� ���������, ��������� ��������� ������ ��� ���� ����������.
// �������� ������� T ����� ��� �������, �� ������� ��������� ���������
template <typename T>
class ArrayPtr {
public:
	// ����������� �� ��������� 
	// ������ ������� ���������
	ArrayPtr() = default;

	// ����������� �� ���� ������ ���������
	explicit ArrayPtr(T* raw_ptr) noexcept : ptr_(raw_ptr) {
	}

	// ����������� ������� ��������� �������
	explicit ArrayPtr(size_t size)
		: ptr_(size > 0 ? new T[size] : nullptr) {
	}

	// Move - ����������� 
	// ������� ����� ������ ����� ����������� �������� ������� �������
	ArrayPtr(ArrayPtr&& other) noexcept : ptr_(std::exchange(other.ptr_, nullptr)) {
	}

	// ������� � ������ ����������� �����������
//	ArrayPtr(const ArrayPtr&) = delete;

	// ����������. ������� ������, �� ������� ��������� ����� ���������.
	~ArrayPtr() {
		delete[] ptr_;
	}

	// �������� ���������� � ���� bool ��������� ������, ��������� �� ����� ���������
	// �� �����-���� ������
	explicit operator bool() const noexcept {
		return ptr_ != nullptr;
	}

	// Move-�������� ������������
	ArrayPtr& operator=(ArrayPtr&& other) noexcept {
		if (this != &other) {
			delete[] ptr_;
			ptr_ = std::exchange(other.ptr_, nullptr);
		}
		return *this;
	}

	// �������� ������������� ���������� ������ �� ������
	// ����������� ���������� std::logic_error, ���� ��������� �������
	T& operator*() const {
		if (ptr_ == nullptr) {
			throw std::logic_error("Null pointer!");
		}
		return *ptr_;
	}

	// �������� -> ������ ���������� ��������� �� ������
	// ����������� ���������� std::logic_error, ���� ��������� �������
	T* operator->() const {
		if (ptr_ == nullptr) {
			throw std::logic_error("Null pointer!");
		}
		return ptr_;
	}

	// �������� �������������� (������ � �������� ������� �� �������)
	T& operator[](size_t index) noexcept {
		return ptr_[index];
	}

	// ����������� �������� �������������� (������ � �������� ������� �� �������)
	const T& operator[](size_t index) const noexcept {
		return ptr_[index];
	}

	// ���������� ����� ���������
	T* GetRawPtr() noexcept {
		return ptr_;
	}

	// ���������� ����� ���������
	const T* GetRawPtr() const noexcept {
		return ptr_;
	}

	// ���������� �������� ��������, �� ������� ��������� ����� ���������.
	// ���������� ������� �������� "������" ��������� � ������������� ���� ptr_ � null
	T* Release() noexcept {
		T* tmp = ptr_;
		ptr_ = nullptr;
		return tmp;
	}

	// ����� ���������� ���� �������� 
	void swap(ArrayPtr& other) noexcept {
		std::swap(ptr_, other.ptr_);
	}

private:
	T* ptr_ = nullptr;
};
