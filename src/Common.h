#pragma once

#include <limits>
#include <cmath>

namespace crt {
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    inline bool almostEqual(T a, T b, T epsilon = std::numeric_limits<T>::epsilon()) {
        return std::abs(a - b) < epsilon;
    }
}