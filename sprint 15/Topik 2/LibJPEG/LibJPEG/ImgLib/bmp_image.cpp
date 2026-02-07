#include "bmp_image.h"
#include "pack_defines.h"

#include <array>
#include <fstream>
#include <string_view>

using namespace std;

namespace img_lib {

PACKED_STRUCT_BEGIN BitmapFileHeader {
    char signature[2];            // "BM"
    uint32_t size;                 // Общий размер файла
    uint16_t reserved1 = 0;        // Зарезервировано
    uint16_t reserved2 = 0;        // Зарезервировано
    uint32_t offset_data;          // Смещение данных изображения
}
PACKED_STRUCT_END

PACKED_STRUCT_BEGIN BitmapInfoHeader {
    uint32_t size;                 // Размер заголовка (40)
    int32_t width;                 // Ширина изображения
    int32_t height;                // Высота изображения (в BMP строки идут снизу вверх)
    uint16_t planes = 1;           // Количество плоскостей (должно быть 1)
    uint16_t bit_count = 24;       // Бит на пиксель (24 для RGB)
    uint32_t compression = 0;      // Тип сжатия (0 - без сжатия)
    uint32_t image_size;           // Размер данных изображения (в байтах)
    int32_t x_pixels_per_meter = 11811;  // Горизонтальное разрешение
    int32_t y_pixels_per_meter = 11811;  // Вертикальное разрешение
    uint32_t colors_used = 0;      // Количество используемых цветов (0 - не определено)
    uint32_t colors_important = 0x1000000; // Количество значимых цветов
}
PACKED_STRUCT_END

// Вычисление отступа строки в BMP
static int GetBMPStride(int w) {
    return 4 * ((w * 3 + 3) / 4);
}

bool SaveBMP(const Path& file, const Image& image) {
    ofstream out(file, ios::binary);
    if (!out.is_open()) {
        return false;
    }

    const int width = image.GetWidth();
    const int height = image.GetHeight();
    const int stride = GetBMPStride(width);
    const int padding = stride - width * 3;
    const int data_size = stride * height;
    const int file_size = data_size + sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

    // Заполняем заголовки
    BitmapFileHeader file_header;
    file_header.signature[0] = 'B';
    file_header.signature[1] = 'M';
    file_header.size = file_size;
    file_header.offset_data = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

    BitmapInfoHeader info_header;
    info_header.size = sizeof(BitmapInfoHeader);
    info_header.width = width;
    info_header.height = height;
    info_header.image_size = data_size;

    // Записываем заголовки
    out.write(reinterpret_cast<const char*>(&file_header), sizeof(file_header));
    out.write(reinterpret_cast<const char*>(&info_header), sizeof(info_header));

    // Временный буфер для строки с паддингом
    vector<char> row_buffer(stride);

    // Записываем строки в обратном порядке (снизу вверх)
    for (int y = height - 1; y >= 0; --y) {
        const Color* pixel = image.GetLine(y);
        for (int x = 0; x < width; ++x) {
            // BMP использует порядок BGR
            row_buffer[x * 3 + 0] = static_cast<char>(pixel[x].b);
            row_buffer[x * 3 + 1] = static_cast<char>(pixel[x].g);
            row_buffer[x * 3 + 2] = static_cast<char>(pixel[x].r);
        }
        // Записываем строку
        out.write(row_buffer.data(), stride);
    }

    return out.good();
}

Image LoadBMP(const Path& file) {
    ifstream in(file, ios::binary);
    if (!in.is_open()) {
        return {};
    }

    // Читаем заголовки
    BitmapFileHeader file_header;
    BitmapInfoHeader info_header;

    in.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));
    in.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));

    // Проверяем подпись
    if (file_header.signature[0] != 'B' || file_header.signature[1] != 'M') {
        return {};
    }

    // Проверяем, что это RGB без сжатия
    if (info_header.bit_count != 24 || info_header.compression != 0) {
        return {};
    }

    const int width = info_header.width;
    const int height = abs(info_header.height); // Высота может быть отрицательной
    const int stride = GetBMPStride(width);

    // Создаем изображение
    Image image(width, height, Color::Black());

    // Временный буфер для строки
    vector<char> row_buffer(stride);

    // Читаем строки в обратном порядке (в файле снизу вверх)
    for (int y = height - 1; y >= 0; --y) {
        in.read(row_buffer.data(), stride);
        if (!in.good()) {
            return {};
        }

        Color* pixel = image.GetLine(y);
        for (int x = 0; x < width; ++x) {
            // BMP использует порядок BGR
            pixel[x].b = static_cast<byte>(row_buffer[x * 3 + 0]);
            pixel[x].g = static_cast<byte>(row_buffer[x * 3 + 1]);
            pixel[x].r = static_cast<byte>(row_buffer[x * 3 + 2]);
        }
    }

    return image;
}

}  // namespace img_lib
