#pragma once
#include "texture.h"

#include <memory>

// Поддерживаемые виды фигур: прямоугольник и эллипс
enum class ShapeType { RECTANGLE, ELLIPSE };

class Shape {
public:
    // Фигура после создания имеет нулевые координаты и размер,
    // а также не имеет текстуры
    explicit Shape(ShapeType type)
        : size_{ 0, 0 }, pos_{ 0, 0 }, type_(type) {
    }

    // Установка позиции фигуры
    void SetPosition(Point pos) {
        pos_ = pos;
    }

    // Установка размера фигуры
    void SetSize(Size size) {
        size_ = size;
    }

    // Устанавка текстуры фигуры
    void SetTexture(std::shared_ptr<Texture> texture) {
        texture_ = std::move(texture);
    }

    // Рисует фигуру на указанном изображении
    // В зависимости от типа фигуры должен рисоваться либо эллипс, либо прямоугольник
    // Пиксели фигуры, выходящие за пределы текстуры, а также в случае, когда текстура не задана,
    // должны отображаться с помощью символа точка '.'
    // Части фигуры, выходящие за границы объекта image, должны отбрасываться.
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
