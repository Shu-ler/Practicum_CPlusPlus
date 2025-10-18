#pragma once
#include "common.h"

class Texture {
public:
    explicit Texture(Image image)
        : image_(std::move(image)) {
    }

    // Возвращает размер изображения
    Size GetSize() const {
        // TODO: GetSize для Texture
        return { 0, 0 };
    }

    // Возвращает цвет пикселя.
    // Если координаты выходят за границы изображения, возвращается пробел
    char GetPixelColor(Point p) const {
        (void)p;
        // TODO: GetPixelColor для Texture
        return ' ';
    }

private:
    Image image_;
};
