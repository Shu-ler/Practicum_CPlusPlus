#pragma once

#include <ostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "cow.h"
#include "geom.h"
#include "tile.h"

class Image {
public:
    /**
     * ������������ ����������� ��������� �������. ���� ������� �� �������� ��������������,
     * ����������� ���������� std::out_of_range.
     */
    explicit Image(Size size, char color = ' ')
        : size_{ size.width > 0 && size.height > 0 ? size
                                                  : throw std::out_of_range("Invalid image size") }
        , size_in_tiles_{ (size_.width + Tile::SIZE - 1) / Tile::SIZE,
                         (size_.height + Tile::SIZE - 1) / Tile::SIZE }
        , tiles_{ static_cast<size_t>(size_in_tiles_.width * size_in_tiles_.height),
                 CoWTile{Tile{color}} } {
    }

    // ���������� ������ ����������� � ��������.
    Size GetSize() const noexcept {
        /* ���������� ����� ��������������. */

        return { 0, 0 };
    }

    /**
     * ���������� ����� ������� � ��������� �����������.���� ���������� ������� �� �������
     * �����������, ���������� �������.
     */
    char GetPixel(Point p) const noexcept {
        /* ���������� ����� ��������������. */

        return ' ';
    }

    /**
     * ����� ����� ������� � ��������� �����������. ���� ���������� ������� �� ������� �����������
     * �������� ������������.
     */
    void SetPixel(Point p, char color) {
        /* ���������� ����� ��������������. */
    }

private:
    using CoWTile = CoW<Tile>;

    // ���������� ���������� ����� �����, � ������� ��������� ������� � ������������ p
    int GetTileIndex(Point p) const noexcept {
        assert((p.x >= 0) && (p.x < size_.width) && (p.y >= 0) && (p.y < size_.height));
        return (p.y / Tile::SIZE) * size_in_tiles_.width + (p.x / Tile::SIZE);
    }

    Size size_;                   // ������ ����������� � ��������.
    Size size_in_tiles_;          // ������ ����������� � ������.
    std::vector<CoWTile> tiles_;  // ����� ����������� (������ �� �������).
};

/**
 * ������� � ����� out ����������� � ���� ��������.
 */
inline void Print(const Image& img, std::ostream& out) {
    const auto size = img.GetSize();
    for (int y = 0; y < size.height; ++y) {
        for (int x = 0; x < size.width; ++x) {
            out.put(img.GetPixel({ x, y }));
        }
        out.put('\n');
    }
}

/**
 * ��������� ����������� �� pixels. ����� ����������� ����������� ��������� \n.
 * ������� �������� ������������ �� ���������� ��������� ������ � ����� ������� �����.
 */
inline Image LoadImage(const std::string& pixels) {
    std::istringstream s(pixels);
    Size size;
    std::string line;
    while (std::getline(s, line)) {
        size.width = std::max(size.width, static_cast<int>(line.length()));
        ++size.height;
    }

    Image img(size);

    s = std::istringstream(pixels);
    for (int y = 0; y < size.height; ++y) {
        if (!std::getline(s, line)) break;

        int x = 0;
        for (char ch : line) {
            img.SetPixel({ x++, y }, ch);
        }
    }

    return img;
}
