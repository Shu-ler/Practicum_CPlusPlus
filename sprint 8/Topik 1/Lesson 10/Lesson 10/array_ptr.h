#include <stdexcept>

// ����� ���������, ��������� ��������� ������ ��� ���� ����������.
// �������� ������� T ����� ��� �������, �� ������� ��������� ���������
template <typename T>
class ArrayPtr {
public:
	// ����������� �� ��������� ������ ������� ���������,
	// ��� ��� ���� ptr_ ����� �������� �� ��������� nullptr
	ArrayPtr() = default;

	// ������ ���������, ����������� �� ���������� raw_ptr.
	// raw_ptr ��������� ���� �� ������, ��������� � ���� ��� ������ new,
	// ���� �������� ������� ����������
	// ������������ noexcept ����������, ��� ����� �� ������� ����������
	explicit ArrayPtr(T* raw_ptr) noexcept {
		ptr_ = raw_ptr;
	}

	explicit ArrayPtr(size_t size)
		: ptr_(size != 0 ? new T[size] : nullptr) {
	}

	// ������� � ������ ����������� �����������
	ArrayPtr(const ArrayPtr&) = delete;

	// ����������. ������� ������, �� ������� ��������� ����� ���������.
	~ArrayPtr() {
		delete ptr_;
	}

	// �������� ���������� � ���� bool ��������� ������, ��������� �� ����� ���������
	// �� �����-���� ������
	explicit operator bool() const noexcept {
		return ptr_ != nullptr;
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

	// ���������� ���������, ���������� ������ ScopedPtr
	T* GetRawPtr() const noexcept {
		return ptr_;
	}

	// ���������� �������� ��������, �� ������� ��������� ����� ���������.
	// ���������� ������� �������� "������" ��������� � ������������� ���� ptr_ � null
	T* Release() noexcept {
		T* tmp = ptr_;
		ptr_ = nullptr;
		return tmp;
	}

private:
	T* ptr_ = nullptr;
};
