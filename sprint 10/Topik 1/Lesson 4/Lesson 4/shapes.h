#pragma once
#include "texture.h"

#include <memory>

// �������������� ���� �����: ������������� � ������
enum class ShapeType { RECTANGLE, ELLIPSE };

class Shape {
public:
    // ������ ����� �������� ����� ������� ���������� � ������,
    // � ����� �� ����� ��������
    explicit Shape(ShapeType type) {
        // TODO: ����������� Shape(ShapeType type)
        (void)type;
    }

    void SetPosition(Point pos) {
        (void)pos;
        // TODO: ����� SetPosition
    }

    void SetSize(Size size) {
        (void)size;
        // TODO: ����� SetSize
    }

    void SetTexture(std::shared_ptr<Texture> texture) {
        (void)texture;
        // TODO: ����� SetTexture
    }

    // ������ ������ �� ��������� �����������
    // � ����������� �� ���� ������ ������ ���������� ���� ������, ���� �������������
    // ������� ������, ��������� �� ������� ��������, � ����� � ������, ����� �������� �� ������,
    // ������ ������������ � ������� ������� ����� '.'
    // ����� ������, ��������� �� ������� ������� image, ������ �������������.
    void Draw(Image& image) const {
        (void)image;
        // TODO: ����� Draw
    }
};
