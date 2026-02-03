#include "ppm_image.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string_view>

using namespace std;

int Sum(img_lib::Color c) {
    return to_integer<int>(c.r) + to_integer<int>(c.g) + to_integer<int>(c.b);
}

// реализуйте оператор Собеля
img_lib::Image Sobel(const img_lib::Image& image) {
    const int w = image.GetWidth();
    const int h = image.GetHeight();

    img_lib::Image result(w, h, img_lib::Color::Black());

    // Заполняем края чёрным (оставляем по умолчанию), обрабатываем внутренние пиксели
    for (int y = 1; y < h - 1; ++y) {
        const img_lib::Color* prev_line = image.GetLine(y - 1);
        const img_lib::Color* curr_line = image.GetLine(y);
        const img_lib::Color* next_line = image.GetLine(y + 1);

        img_lib::Color* out_line = result.GetLine(y);

        for (int x = 1; x < w - 1; ++x) {
            // Яркости соседних пикселей
            int tl = Sum(prev_line[x - 1]); // top-left
            int tc = Sum(prev_line[x]);     // top-center
            int tr = Sum(prev_line[x + 1]); // top-right
            int cl = Sum(curr_line[x - 1]); // center-left
            int cr = Sum(curr_line[x + 1]); // center-right
            int bl = Sum(next_line[x - 1]); // bottom-left
            int bc = Sum(next_line[x]);     // bottom-center
            int br = Sum(next_line[x + 1]); // bottom-right

            // Вычисляем компоненты градиента
            int gx = -tl - 2 * tc - tr + bl + 2 * bc + br;
            int gy = -tl - 2 * cl - bl + tr + 2 * cr + br;

            // Модуль градиента
            double magnitude = sqrt(gx * gx + gy * gy);

            // Ограничиваем в диапазоне [0, 255]
            magnitude = clamp(magnitude, 0.0, 255.0);

            // Преобразуем в byte
            std::byte val = static_cast<std::byte>(static_cast<unsigned char>(magnitude));

            // Устанавливаем все три компоненты цвета
            out_line[x] = img_lib::Color{ val, val, val, std::byte{255} };
        }
    }

    // Крайние строки и столбцы остаются чёрными (по умолчанию)

    return result;
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

    image = Sobel(image);

    if (!img_lib::SavePPM(argv[2], image)) {
        cerr << "Error saving image"sv << endl;
        return 3;
    }

    cout << "Image saved successfully!"sv << endl;
}
