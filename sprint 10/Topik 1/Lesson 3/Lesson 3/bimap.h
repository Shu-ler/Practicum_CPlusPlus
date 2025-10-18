#pragma once

#include <optional>
#include <string_view>

class BiMap {
public:
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
    /* ����� ������ ������������ ������ Pimpl, ������������ ����������� � ����������� */
};
