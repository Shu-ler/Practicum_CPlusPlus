#pragma once

#include <optional>
#include <string_view>
#include <memory>

class BiMap {
public:
    BiMap();

    // ���������� �����������
    BiMap(const BiMap& other);

    // ������������ �����������
    BiMap(BiMap&& other) noexcept;

    ~BiMap();

    // ���������� ������������
    BiMap& operator=(const BiMap& other);

    // ������������ ������������
    BiMap& operator=(BiMap&& other) noexcept;
    
    /**
     * ��������� � ������� ���� "����-��������".
     * � ������ ������ ���������� true.
     * ���������� false, ���� ���� ��� �������� ���� ����� ���������.
     *
     * ����� ������������ ������� �������� ������������ ����������
     */
    bool Add(std::string_view key, std::string_view value);

    /**
     * ���������� ��������, ��������� � ������, ���� nullopt, ���� ������ ����� ���
     */
    std::optional<std::string_view> FindValue(std::string_view key) const noexcept;

    /**
     * ���������� ����, ��������� � value, ���� nullopt, ���� ������ �������� ���
     */
    std::optional<std::string_view> FindKey(std::string_view value) const noexcept;

private:
    // ��������� ��������� ��������� Impl, �� ��������� ����� � ����������
    struct Impl;
    // unique_ptr ������������� ������ ��������� � ������� ������
    std::unique_ptr<Impl> impl_;
};
