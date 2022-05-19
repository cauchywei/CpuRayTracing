#pragma once

#include <array>
#include <cmath>

namespace cry::math {

    template<typename T, size_t N>
    class Vector {
    public:
        static_assert(N > 0, "Vector size must be greater than 0");
        static_assert(std::is_arithmetic<T>::value, "Vector type must be arithmetic");
        static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value,
                      "Vector type must be float or double");

        Vector() = default;

        Vector(const Vector&) = default;

        Vector(Vector&&) noexcept = default;

        ~Vector() = default;

        Vector& operator=(const Vector&) = default;


        template<typename = std::enable_if_t<N >= 1>>
        T getX() const {
            return _data[0];
        }

        template<typename = std::enable_if_t<N >= 1>>
        void setX(T x) {
            _data[0] = x;
        }

        template<typename = std::enable_if_t<N >= 2>>
        T getY() const {
            return _data[1];
        }

        template<typename = std::enable_if_t<N >= 2>>
        void setY(T y) {
            _data[1] = y;
        }

        template<typename = std::enable_if_t<N >= 3>>
        T getZ() const {
            return _data[2];
        }

        template<typename = std::enable_if_t<N >= 3>>
        void setZ(T z) {
            _data[2] = z;
        }

        template<typename = std::enable_if_t<N >= 4>>
        T getW() const {
            return _data[3];
        }

        template<typename = std::enable_if_t<N >= 4>>
        void setW(T w) {
            _data[3] = w;
        }

        [[nodiscard]] size_t getSize() const {
            return N;
        }

        [[nodiscard]] double getLength() const {
            double length = 0;
            for (size_t i = 0; i < N; ++i) {
                length += _data[i] * _data[i];
            }
            return std::sqrt(length);
        }

        Vector<T, N> normalize() const {
            double length = getLength();
            Vector<T, N> result;
            for (size_t i = 0; i < N; ++i) {
                result._data[i] = _data[i] / length;
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
                if (_data[i] != other._data[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const Vector<T, N>& other) const {
            return !(*this == other);
        }

        [[nodiscard]] T dot(const Vector<T, N>& other) const {
            T result = 0;
            for (size_t i = 0; i < N; ++i) {
                result += _data[i] * other._data[i];
            }
            return result;
        }

        template<typename = std::enable_if_t<N == 3>>
        [[nodiscard]] Vector<T, N> cross(const Vector<T, N>& other) const {
            Vector<T, N> result;
            result._data[0] = _data[1] * other._data[2] - _data[2] * other._data[1];
            result._data[1] = _data[2] * other._data[0] - _data[0] * other._data[2];
            result._data[2] = _data[0] * other._data[1] - _data[1] * other._data[0];
            return result;
        }

        [[nodiscard]] T operator[](size_t index) const {
            return _data[index];
        }

    private:
        std::array<T, N> _data;
    };

    template<typename Type = float>
    using Vector2 = Vector<Type, 2>;
    using Vector2f = Vector2<float>;
    using Vector2d = Vector2<double>;

    template<typename Type = float>
    using Vector3 = Vector<Type, 3>;
    using Vector3f = Vector3<float>;
    using Vector3d = Vector3<double>;
}