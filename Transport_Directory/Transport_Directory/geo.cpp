#define _USE_MATH_DEFINES
#include "geo.h"

#include <cmath>

namespace geo {

    double ComputeDistance(Coordinates from, Coordinates to) {
        using namespace std;

        return acos(sin(from.lat * DEG_TO_RAD) * sin(to.lat * DEG_TO_RAD)
            + cos(from.lat * DEG_TO_RAD) * cos(to.lat * DEG_TO_RAD) 
            * cos(abs(from.lng - to.lng) * DEG_TO_RAD))
            * EARTH_RADIUS_METERS;
    }
}  // namespace geo
