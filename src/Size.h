#pragma once

#include "Vector.h"

namespace crt {

    template<typename T>
    class Size : public Vector<T, 2> {
    public:
        using SuperType = Vector<T, 2>;
    public:
        constexpr Size() : SuperType() {}

        constexpr Size(T width, T height) : SuperType(width, height) {}

        constexpr Size(const Size &rhs) : SuperType(rhs._data) {}

        constexpr Size(Size &&rhs) noexcept: SuperType(std::move(rhs)) {}

        constexpr const T &getWidth() const {
            return SuperType::getX();
        }

        void setWidth(T width) {
            SuperType::setX(width);
        }

        constexpr const T &getHeight() const {
            return SuperType::getY();
        }

        void setHeight(T height) {
            SuperType::setY(height);
        }

    };

    using SizeI = Size<int>;
    using SizeF = Size<float>;
    using SizeD = Size<double>;
}