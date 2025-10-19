#pragma once
#include <array>
#include <cassert>

#include "geom.h"

class Tile {
public:
    // ������ ����� 8*8 ��������.
    constexpr static int SIZE = 8;

    // ����������� �� ���������. ��������� ���� ��������� ������.
    Tile(char color = ' ') noexcept {
        for (auto& row : pixels_) {
            for (auto& pixel : row) {
                pixel = color;
            }
        }

        // -------------- �� ������� ------------
        assert(instance_count_ >= 0);
        ++instance_count_;  // ����������� ������� ������ (��� ����� ������������).
        // -------------- �� ������� ------------
    }

    Tile(const Tile& other) {
        pixels_ = other.pixels_;

        // -------------- �� ������� ------------
        assert(instance_count_ >= 0);
        ++instance_count_;  // ����������� ������� ������ (��� ����� ������������).
        // -------------- �� ������� ------------
    }

    ~Tile() {
        // -------------- �� ������� ------------
        --instance_count_;  // ��������� ������� ������.
        assert(instance_count_ >= 0);
        // -------------- �� ������� ------------
    }

    /**
     * �������� ���� ������� �����.
     * ���� ���������� ������� �� ������� �����, ����� ������ �� ������.
     */
    void SetPixel(Point p, char color) noexcept {
        if (p.x < 0 || p.x >= SIZE || p.y < 0 || p.y >= SIZE) {
            return;
        }
        pixels_[p.y][p.x] = color;
    }

    /**
     * ���������� ���� �������. ���� ���������� ������� �� ������� �����, ������������ ������.
     */
    char GetPixel(Point p) const noexcept {
        if (p.x < 0 || p.x >= SIZE || p.y < 0 || p.y >= SIZE) {
            return ' ';
        }
        return pixels_[p.y][p.x];
    }

    // ���������� ���������� ���������� ������ Tile � ���������.
    static int GetInstanceCount() noexcept {
        // -------------- �� ������� ------------
        return instance_count_;
        // -------------- �� ������� ------------
    }

private:
    // -------------- �� ������� ------------
    inline static int instance_count_ = 0;
    // -------------- �� ������� ------------

    std::array<std::array<char, SIZE>, SIZE> pixels_;
};