#pragma once
#include "common.h"

class Texture {
public:
    explicit Texture(Image image)
        : image_(std::move(image)) {
    }

    // Возвращает размер изображения
    Size GetSize() const {
        return GetImageSize(image_);
    }

    // Возвращает цвет пикселя.
    // Если координаты выходят за границы изображения, возвращается пробел
    char GetPixelColor(Point p) const {
        if (IsPointInSize(p, GetSize())) {
            return image_.at(p.y).at(p.x);
        }
        else {
            return ' ';
        }
    }

private:
    Image image_;
};
