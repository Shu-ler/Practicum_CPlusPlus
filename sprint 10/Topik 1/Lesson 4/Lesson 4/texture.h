#pragma once
#include "common.h"

class Texture {
public:
    explicit Texture(Image image)
        : image_(std::move(image)) {
    }

    // ���������� ������ �����������
    Size GetSize() const {
        // ��������. ���������� ����� ��������������
        return { 0, 0 };
    }

    // ���������� ���� �������.
    // ���� ���������� ������� �� ������� �����������, ������������ ������
    char GetPixelColor(Point p) const {
        (void)p;
        // ��������. ���������� ����� ��������������
        return ' ';
    }

private:
    Image image_;
};
