#include "ppm_image.h"

#include <iostream>
#include <string_view>

using namespace std;

/*
 * Три следующие фукнкции - задача 3
 */

// Функция обращения одной компоненты (std::byte)
std::byte Negate(std::byte b) {
    return std::byte{ static_cast<unsigned char>(255 - std::to_integer<int>(b)) };
}

// Функция обращения цвета (без изменения прозрачности)
img_lib::Color Negate(img_lib::Color c) {
    return img_lib::Color{
        Negate(c.r),
        Negate(c.g),
        Negate(c.b),
        c.a  // прозрачность не меняется
    };
}

// Функция инвертирует изображение на месте, построчно
void NegateInplace(img_lib::Image& image) {
    const int width = image.GetWidth();
    const int height = image.GetHeight();

    for (int y = 0; y < height; ++y) {
        img_lib::Color* line = image.GetLine(y);
        for (int x = 0; x < width; ++x) {
            line[x] = Negate(line[x]);
        }
    }
}

int main(int argc, const char** argv) {
    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <input image> <output image>"sv << endl;
        return 1;
    }

    auto image = img_lib::LoadPPM(argv[1]);
    if (!image) {
        cerr << "Error loading image"sv << endl;
        return 2;
    }

    // Инвертируем цвета изображения
    NegateInplace(image);

    if (!img_lib::SavePPM(argv[2], image)) {
        cerr << "Error saving image"sv << endl;
        return 3;
    }

    cout << "Image saved successfully!"sv << endl;
}