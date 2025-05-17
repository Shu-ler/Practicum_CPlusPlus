#pragma once

#include <iostream>
#include <sstream>
#include <vector>

// Вывод для pair.
template <typename F, typename S>
std::ostream& operator << (std::ostream &os, const std::pair<F, S> &p) {
    using namespace std::literals;
    return os << "("s << p.first << "; "s << p.second << ")"s;
}

template<typename T> // Преобразует в строку.
std::string ToString(const std::vector<T>& x) {
    using namespace std::literals;

   std::ostringstream s;

   for (auto el : x) {
    s << el << ", "s;
   }

   auto res = s.str();
   auto res_length = res.length();
   if ( res_length > 1)
   {
        res = res.substr(0, res_length - 2);
   }
   return res;
}
