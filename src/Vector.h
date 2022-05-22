#pragma once

#include <array>
#include <cmath>
#include "Common.h"

namespace crt {

    template<typename T, size_t N>
    class Vector {
    public:
        static_assert(N > 0, "Vector size must be greater than 0");
        static_assert(std::is_arithmetic<T>::value, "Vector type must be arithmetic");

        constexpr Vector() = default;

        constexpr Vector(const Vector&) = default;

        constexpr Vector(Vector&&) noexcept = default;

        constexpr explicit Vector(const std::array<T, N>& data) : _data(data) {}

        constexpr explicit Vector(T x) : _data{std::move(x)} {
            static_assert(N == 1);
        }

        constexpr Vector(T x, T y) : _data{std::move(x), std::move(y)} {
            static_assert(N >= 2);
        }

        constexpr Vector(T x, T y, T z) : _data{std::move(x), std::move(y), std::move(z)} {
            static_assert(N >= 3);
        }

        constexpr Vector(T x, T y, T z, T w) : _data{std::move(x), std::move(y), std::move(z), std::move(w)} {
            static_assert(N >= 4);
        }

        template<typename ...Args>
        constexpr explicit Vector(Args&& ... args) : _data{std::forward<Args>(args)...} {}


        ~Vector() = default;

        Vector& operator=(const Vector& rhs) {
            _data = rhs._data;
            return *this;
        }

        Vector& operator=(Vector&& rhs) noexcept {
            _data = std::move(rhs._data);
            return *this;
        }

        constexpr const T& getX() const {
            static_assert(N >= 1);
            return _data[0];
        }

        void setX(T x) {
            static_assert(N >= 1);
            _data[0] = x;
        }

        constexpr const T& getY() const {
            static_assert(N >= 2);
            return _data[1];
        }

        void setY(T y) {
            static_assert(N >= 2);
            _data[1] = y;
        }

        constexpr const T& getZ() const {
            static_assert(N >= 3);
            return _data[2];
        }

        void setZ(T z) {
            static_assert(N >= 3);
            _data[2] = z;
        }

        constexpr const T& getW() const {
            static_assert(N >= 4);
            return _data[3];
        }

        void setW(T w) {
            static_assert(N >= 4);
            _data[3] = w;
        }

        [[nodiscard]] constexpr size_t getSize() const {
            return N;
        }

        [[nodiscard]] constexpr float getLength() const {
            double length = 0;
            for (size_t i = 0; i < N; ++i) {
                length += _data[i] * _data[i];
            }
            return static_cast<float>(std::sqrt(length));
        }

        [[nodiscard]] constexpr bool isZero() const {
            for (size_t i = 0; i < N; ++i) {
                if (!almostEqual(_data[i], 0.0f)) {
                    return false;
                }
            }
            return true;
        }

        [[nodiscard]] constexpr Vector<T, N> getNormalized() const {
            Vector<T, N> result;
            float length = getLength();
            for (size_t i = 0; i < N; ++i) {
                result._data[i] = static_cast<T>(_data[i] / length);
            }
            return result;
        }

        [[nodiscard]] constexpr const T& operator[](size_t index) const {
            return _data[index];
        }

        Vector<T, N> constexpr normalize() const {
            const auto length = getLength();
            assert(!almostEqual(length, static_cast<decltype(length)>(0)));
            Vector<T, N> result;
            for (size_t i = 0; i < N; ++i) {
                result._data[i] = _data[i] / length;
            }
            return result;
        }

        Vector<T, N> constexpr max(const Vector<T, N>& other) const {
            Vector<T, N> result;
            for (size_t i = 0; i < N; ++i) {
                result._data[i] = std::max(_data[i], other._data[i]);
            }
            return result;
        }

        Vector<T, N> constexpr min(const Vector<T, N>& other) const {
            Vector<T, N> result;
            for (size_t i = 0; i < N; ++i) {
                result._data[i] = std::min(_data[i], other._data[i]);
            }
            return result;
        }

        Vector<T, N>& operator+=(const Vector<T, N>& other) {
            for (size_t i = 0; i < N; ++i) {
                _data[i] += other._data[i];
            }
            return *this;
        }

        Vector<T, N>& operator-=(const Vector<T, N>& other) {
            for (size_t i = 0; i < N; ++i) {
                _data[i] -= other._data[i];
            }
            return *this;
        }

        Vector<T, N>& operator*=(const Vector<T, N>& other) {
            for (size_t i = 0; i < N; ++i) {
                _data[i] *= other._data[i];
            }
            return *this;
        }

        Vector<T, N>& operator/=(const Vector<T, N>& other) {
            for (size_t i = 0; i < N; ++i) {
                _data[i] /= other._data[i];
            }
            return *this;
        }

        Vector<T, N>& operator/=(T scalar) {
            for (size_t i = 0; i < N; ++i) {
                _data[i] /= scalar;
            }
            return *this;
        }

        Vector<T, N>& operator+=(T scalar) {
            for (size_t i = 0; i < N; ++i) {
                _data[i] += scalar;
            }
            return *this;
        }

        Vector<T, N>& operator-=(T scalar) {
            for (size_t i = 0; i < N; ++i) {
                _data[i] -= scalar;
            }
            return *this;
        }

        Vector<T, N>& operator*=(T scalar) {
            for (size_t i = 0; i < N; ++i) {
                _data[i] *= scalar;
            }
            return *this;
        }

        Vector<T, N> operator+(const Vector<T, N>& other) const {
            Vector<T, N> result = *this;
            result += other;
            return result;
        }

        Vector<T, N> operator-(const Vector<T, N>& other) const {
            Vector<T, N> result = *this;
            result -= other;
            return result;
        }

        Vector<T, N> operator*(const Vector<T, N>& other) const {
            Vector<T, N> result = *this;
            result *= other;
            return result;
        }

        Vector<T, N> operator/(const Vector<T, N>& other) const {
            Vector<T, N> result = *this;
            result /= other;
            return result;
        }

        Vector<T, N> operator+(T scalar) const {
            Vector<T, N> result = *this;
            result += scalar;
            return result;
        }

        Vector<T, N> operator-(T scalar) const {
            Vector<T, N> result = *this;
            result -= scalar;
            return result;
        }

        Vector<T, N> operator*(T scalar) const {
            Vector<T, N> result = *this;
            result *= scalar;
            return result;
        }

        Vector<T, N> operator/(T scalar) const {
            Vector<T, N> result = *this;
            result /= scalar;
            return result;
        }

        bool operator==(const Vector<T, N>& other) const {
            for (size_t i = 0; i < N; ++i) {
                if (!almostEqual(_data[i], other._data[i])) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const Vector<T, N>& other) const {
            return !operator==(other);
        }

        [[nodiscard]] T constexpr dot(const Vector<T, N>& other) const {
            T result = 0;
            for (size_t i = 0; i < N; ++i) {
                result += _data[i] * other._data[i];
            }
            return result;
        }

        [[nodiscard]] constexpr Vector<T, N> cross(const Vector<T, N>& other) const {
            static_assert(N == 3, "Cross product only defined for 3D vectors");
            Vector<T, N> result;
            result._data[0] = _data[1] * other._data[2] - _data[2] * other._data[1];
            result._data[1] = _data[2] * other._data[0] - _data[0] * other._data[2];
            result._data[2] = _data[0] * other._data[1] - _data[1] * other._data[0];
            return result;
        }

    private:
        std::array<T, N> _data;
    };

    template<class Type = float>
    using Vector2 = Vector<Type, 2>;
    using Vector2f = Vector2<float>;
    using Vector2d = Vector2<double>;

    template<class Type = float>
    using Vector3 = Vector<Type, 3>;
    using Vector3f = Vector3<float>;
    using Vector3d = Vector3<double>;
}