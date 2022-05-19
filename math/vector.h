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

        explicit Vector(const std::array<T, N>& data) : _data(data) {}

        template<typename ...Args>
        explicit Vector(Args&& ... args) : _data{std::forward<Args>(args)...} {}

        ~Vector() = default;

        Vector& operator=(const Vector& rhs) {
            _data = rhs._data;
            return *this;
        }

        Vector& operator=(Vector&& rhs) noexcept {
            _data = std::move(rhs._data);
            return *this;
        }

        const T& getX() const {
            static_assert(N >= 1);
            return _data[0];
        }

        void setX(T x) {
            static_assert(N >= 1);
            _data[0] = x;
        }

        const T& getY() const {
            static_assert(N >= 2);
            return _data[1];
        }

        void setY(T y) {
            static_assert(N >= 2);
            _data[1] = y;
        }

        const T& getZ() const {
            static_assert(N >= 3);
            return _data[2];
        }

        void setZ(T z) {
            static_assert(N >= 3);
            _data[2] = z;
        }

        const T& getW() const {
            static_assert(N >= 4);
            return _data[3];
        }

        void setW(T w) {
            static_assert(N >= 4);
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
            assert(!almostEqual(length, 0));
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
            return !(*this == other);
        }

        [[nodiscard]] T dot(const Vector<T, N>& other) const {
            T result = 0;
            for (size_t i = 0; i < N; ++i) {
                result += _data[i] * other._data[i];
            }
            return result;
        }

        [[nodiscard]] Vector<T, N> cross(const Vector<T, N>& other) const {
            static_assert(N == 3, "Cross product only defined for 3D vectors");
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
        bool almostEqual(T a, T b) const {
            return std::abs(a - b) < std::numeric_limits<T>::epsilon();
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