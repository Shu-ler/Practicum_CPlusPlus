#pragma once
#include "common.h"

class Texture {
public:
    explicit Texture(Image image)
        : image_(std::move(image)) {
    }

    // ���������� ������ �����������
    Size GetSize() const {
        // TODO: GetSize ��� Texture
        return { 0, 0 };
    }

    // ���������� ���� �������.
    // ���� ���������� ������� �� ������� �����������, ������������ ������
    char GetPixelColor(Point p) const {
        (void)p;
        // TODO: GetPixelColor ��� Texture
        return ' ';
    }

private:
    Image image_;
};
