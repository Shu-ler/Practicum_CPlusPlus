#pragma once
#include "texture.h"

#include <memory>

// Поддерживаемые виды фигур: прямоугольник и эллипс
enum class ShapeType { RECTANGLE, ELLIPSE };

class Shape {
public:
    // Фигура после создания имеет нулевые координаты и размер,
    // а также не имеет текстуры
    explicit Shape(ShapeType type) {
        // TODO: Конструктор Shape(ShapeType type)
        (void)type;
    }

    void SetPosition(Point pos) {
        (void)pos;
        // TODO: Метод SetPosition
    }

    void SetSize(Size size) {
        (void)size;
        // TODO: Метод SetSize
    }

    void SetTexture(std::shared_ptr<Texture> texture) {
        (void)texture;
        // TODO: Метод SetTexture
    }

    // Рисует фигуру на указанном изображении
    // В зависимости от типа фигуры должен рисоваться либо эллипс, либо прямоугольник
    // Пиксели фигуры, выходящие за пределы текстуры, а также в случае, когда текстура не задана,
    // должны отображаться с помощью символа точка '.'
    // Части фигуры, выходящие за границы объекта image, должны отбрасываться.
    void Draw(Image& image) const {
        (void)image;
        // TODO: Метод Draw
    }
};
