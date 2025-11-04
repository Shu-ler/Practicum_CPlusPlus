#define _USE_MATH_DEFINES
#include "geo.h"

#include <cmath>

namespace geo {

    double ComputeDistance(Coordinates from, Coordinates to) {
        using namespace std;

        static const double dr = M_PI / 180.0;      // Коэффициент перевода градусов в радианы
        static const size_t earth_radius = 6371000; // Радиус Земли в метрах

        return acos(sin(from.lat * dr) * sin(to.lat * dr)
            + cos(from.lat * dr) * cos(to.lat * dr) * cos(abs(from.lng - to.lng) * dr))
            * earth_radius;
    }
}  // namespace geo
