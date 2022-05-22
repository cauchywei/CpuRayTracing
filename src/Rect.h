#pragma once

#include "Vector.h"
#include "Size.h"

namespace crt {

    template<typename T>
    class Rect {
    public:
        constexpr Rect() = default;

        constexpr Rect(T left, T top, T width, T height) : _data{left, top, width, height} {}

        constexpr Rect(const Rect& rhs) : _data(rhs._data) {}

        constexpr Rect(Rect&& rhs) noexcept: _data(std::move(rhs._data)) {}

        constexpr const T& getLeft() const {
            return _data[0];
        }

        void setLeft(T left) {
            _data[0] = left;
        }

        constexpr const T& getTop() const {
            return _data[1];
        }

        void setTop(T top) {
            _data[1] = top;
        }

        constexpr const T& getRight() const {
            return _data[2];
        }

        void setRight(T right) {
            _data[2] = right;
        }

        constexpr const T& getBottom() const {
            return _data[3];
        }

        void setBottom(T bottom) {
            _data[3] = bottom;
        }

        constexpr const T& getWidth() const {
            return getRight() - getLeft();
        }

        constexpr const T& getHeight() const {
            return getBottom() - getTop();
        }

        bool operator==(const Rect& rhs) const {
            return _data == rhs._data;
        }

        bool operator!=(const Rect& rhs) const {
            return _data != rhs._data;
        }

    private:
        Vector<T, 4> _data;
    };

    using RectI = Rect<int>;
    using RectF = Rect<float>;
    using RectD = Rect<double>;
}
