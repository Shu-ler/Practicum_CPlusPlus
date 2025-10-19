#pragma once
#include "texture.h"

#include <memory>

// �������������� ���� �����: ������������� � ������
enum class ShapeType { RECTANGLE, ELLIPSE };

class Shape {
public:
    // ������ ����� �������� ����� ������� ���������� � ������,
    // � ����� �� ����� ��������
    explicit Shape(ShapeType type)
        : size_{ 0, 0 }, pos_{ 0, 0 }, type_(type) {
    }

    // ��������� ������� ������
    void SetPosition(Point pos) {
        pos_ = pos;
    }

    // ��������� ������� ������
    void SetSize(Size size) {
        size_ = size;
    }

    // ��������� �������� ������
    void SetTexture(std::shared_ptr<Texture> texture) {
        texture_ = std::move(texture);
    }

    // ������ ������ �� ��������� �����������
    // � ����������� �� ���� ������ ������ ���������� ���� ������, ���� �������������
    // ������� ������, ��������� �� ������� ��������, � ����� � ������, ����� �������� �� ������,
    // ������ ������������ � ������� ������� ����� '.'
    // ����� ������, ��������� �� ������� ������� image, ������ �������������.
    void Draw(Image& image) const {
        for (int y = 0; y < size_.height; ++y) {
            for (int x = 0; x < size_.width; ++x) {
                const Point local_point{ x, y };
                const Point image_point{ x + pos_.x, y + pos_.y };
                if (IsPointInShape(local_point)) {
                    SetPixelColor(image, image_point, GetTextureColor(local_point));
                }
            }
        }
    }

private:
    
    char GetTextureColor(Point local_point) const {
        if (texture_ && IsPointInSize(local_point, texture_->GetSize())) {
            return texture_->GetPixelColor(local_point);
        }
        else {
            return '.';
        }
    }

    bool IsPointInShape(Point local_point) const {
        switch (type_) {
        case ShapeType::RECTANGLE:
            return IsPointInSize(local_point, size_);
        case ShapeType::ELLIPSE:
            return IsPointInEllipse(local_point, size_);
        default:
            return false;
        }
    }

private:
    Size size_;
    Point pos_;
    ShapeType type_;
    std::shared_ptr<const Texture> texture_;
};
