#pragma once

#include <limits>
#include <cmath>

namespace cry::math {
    template<typename T>
    inline bool almostEqual(T a, T b, T epsilon = std::numeric_limits<T>::epsilon()) {
        return std::abs(a - b) < epsilon;
    }
}