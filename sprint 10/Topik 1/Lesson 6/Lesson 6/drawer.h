#pragma once

#include <cstdlib>

#include "image.h"

namespace detail {

    inline int Sign(int value) {
        return (0 < value) - (value < 0);
    }

    /**
     * ������ ������ ������� (��� �������� |to.y - from.x| >= |to.x - from.x|).
     */
    inline void DrawSteepLine(Image& image, Point from, Point to, char color) {
        const int delta_x = std::abs(to.x - from.x);
        const int delta_y = std::abs(to.y - from.y);

        assert(delta_y >= delta_x);

        if (from.y > to.y) {  // ������ ������� ������ ������ ����.
            std::swap(from, to);
        }

        const int step_x = Sign(to.x - from.x);  // ��� �� ��� X (-1, 0 ��� 1).
        const int error_threshold = delta_y + 1;  // ����� ������ ���������� ���������� X.
        const int delta_err = delta_x + 1;  // ��� ���������� ������.

        // ����� ��������� �������� ������ ���������� �� � 0, � � delta_err/2,
        // ������� ���������� ��������.
        int error = delta_err / 2;

        for (Point p = from; p.y <= to.y; ++p.y) {
            image.SetPixel({ p.x, p.y }, color);
            assert((p.y != to.y) || (p.x == to.x));

            error += delta_err;  // ����������� ������ ���������� ���������� X.

            if (error >= error_threshold) {  // ���� ����� �� ������� ������� ���������� X
                p.x += step_x;  // ��������� � ��������� ���������� X
                error -= error_threshold;  // ���������� ������
            }
        }
    }

    /**
     * ������ ������� ������� (��� �������� |to.y - from.x| >= |to.y - from.y|).
     */
    inline void DrawSlopeLine(Image& image, Point from, Point to, char color) {
        const int delta_x = std::abs(to.x - from.x);
        const int delta_y = std::abs(to.y - from.y);

        assert(delta_x >= delta_y);

        if (from.x > to.x) {  // ������� ������� ������ ����� �������.
            std::swap(from, to);
        }

        // ������� ������� �������� �� �������� � �������.

        const int step_y = Sign(to.y - from.y);
        const int error_threshold = delta_x + 1;
        const int delta_err = delta_y + 1;

        int error = delta_err / 2;

        for (Point p = from; p.x <= to.x; ++p.x) {
            image.SetPixel({ p.x, p.y }, color);
            assert((p.x != to.x) || (p.y == to.y));

            error += delta_err;

            if (error >= error_threshold) {
                p.y += step_y;
                error -= error_threshold;
            }
        }
    }

}  // namespace detail

/**
 * ������ ������� ������ ����� ����� ������� from � to ������ color �� ����������� Image.
 *
 * ��� ��������� ������������ �������� ����������.
 * (https://ru.wikipedia.org/wiki/��������_����������)
 */
inline void DrawLine(Image& image, Point from, Point to, char color) {
    const int delta_x = std::abs(to.x - from.x);
    const int delta_y = std::abs(to.y - from.y);

    if (delta_y > delta_x) {  // ������� ������.
        detail::DrawSteepLine(image, from, to, color);
    }
    else {  // ������� �������.
        detail::DrawSlopeLine(image, from, to, color);
    }
}
