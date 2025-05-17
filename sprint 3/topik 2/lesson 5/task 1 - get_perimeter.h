#pragma once

template<typename T>
auto GetPerimeter(T a) {
    return a * 4;
}

template<typename T, typename U>
auto GetPerimeter(T a, U b) {
    return (a + b) * 2;
}

template<typename T, typename U, typename Q, typename R>
auto GetPerimeter(T a, U b, Q c, R d) {
    return a + b + c + d;
}
