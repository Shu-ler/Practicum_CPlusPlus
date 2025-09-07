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
    explicit SimpleVector(size_t size) {
        // �������� ���� ������������ ��������������
    }

    // ������ ������ �� size ���������, ������������������ ��������� value
    SimpleVector(size_t size, const Type& value) {
        // �������� ���� ������������ ��������������
    }

    // ������ ������ �� std::initializer_list
    SimpleVector(std::initializer_list<Type> init) {
        // �������� ���� ������������ ��������������
    }

    // ���������� ���������� ��������� � �������
    size_t GetSize() const noexcept {
        // �������� ���� ��������������
        return 0;
    }

    // ���������� ����������� �������
    size_t GetCapacity() const noexcept {
        // �������� ���� ��������������
        return 0;
    }

    // ��������, ������ �� ������
    bool IsEmpty() const noexcept {
        return true;
    }

    // ���������� ������ �� ������� � �������� index
    Type& operator[](size_t index) noexcept {
        // �������� ���� ��������������
    }

    // ���������� ����������� ������ �� ������� � �������� index
    const Type& operator[](size_t index) const noexcept {
        // �������� ���� ��������������
    }

    // ���������� ����������� ������ �� ������� � �������� index
    // ����������� ���������� std::out_of_range, ���� index >= size
    Type& At(size_t index) {
        // �������� ���� ��������������
    }

    // ���������� ����������� ������ �� ������� � �������� index
    // ����������� ���������� std::out_of_range, ���� index >= size
    const Type& At(size_t index) const {
        // �������� ���� ��������������
    }

    // �������� ������ �������, �� ������� ��� �����������
    void Clear() noexcept {
        // �������� ���� ��������������
    }

    // �������� ������ �������.
    // ��� ���������� ������� ����� �������� �������� �������� �� ��������� ��� ���� Type
    void Resize(size_t new_size) {
        // �������� ���� ��������������
    }

    // ���������� �������� �� ������ �������
    // ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
    Iterator begin() noexcept {
        // �������� ���� ��������������
    }

    // ���������� �������� �� �������, ��������� �� ���������
    // ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
    Iterator end() noexcept {
        // �������� ���� ��������������
    }

    // ���������� ����������� �������� �� ������ �������
    // ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
    ConstIterator begin() const noexcept {
        // �������� ���� ��������������
    }

    // ���������� �������� �� �������, ��������� �� ���������
    // ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
    ConstIterator end() const noexcept {
        // �������� ���� ��������������
    }

    // ���������� ����������� �������� �� ������ �������
    // ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
    ConstIterator cbegin() const noexcept {
        // �������� ���� ��������������
    }

    // ���������� �������� �� �������, ��������� �� ���������
    // ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
    ConstIterator cend() const noexcept {
        // �������� ���� ��������������
    }
};
