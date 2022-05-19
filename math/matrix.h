#pragma once

#include <cstddef>

namespace cry::math {
    template<typename T, size_t R, size_t C>
    class Matrix {
    public:
        Matrix() = default;

        Matrix(const Matrix &) = default;

        Matrix(Matrix &&) noexcept = default;

        Matrix &operator=(const Matrix &) = default;

        Matrix &operator=(Matrix &&) noexcept = default;

        T &operator()(size_t r, size_t c) {
            return _data[r * C + c];
        }

        const T &operator()(size_t r, size_t c) const {
            return _data[r * C + c];
        }

        T &operator[](size_t i) {
            return _data[i];
        }

        const T &operator[](size_t i) const {
            return _data[i];
        }

        Matrix &operator+=(const Matrix &rhs) {
            for (size_t i = 0; i < R * C; ++i) {
                _data[i] += rhs._data[i];
            }
            return *this;
        }

        Matrix &operator-=(const Matrix &rhs) {
            for (size_t i = 0; i < R * C; ++i) {
                _data[i] -= rhs._data[i];
            }
            return *this;
        }

        Matrix &operator*=(const Matrix &rhs) {
            Matrix<T, R, C> result;
            for (size_t r = 0; r < R; ++r) {
                for (size_t c = 0; c < C; ++c) {
                    result(r, c) = 0;
                    for (size_t i = 0; i < C; ++i) {
                        result(r, c) += (*this)(r, i) * rhs(i, c);
                    }
                }
            }
            *this = result;
            return *this;
        }

        Matrix &operator*=(T rhs) {
            for (size_t i = 0; i < R * C; ++i) {
                _data[i] *= rhs;
            }
            return *this;
        }

        Matrix &operator/=(T rhs) {
            for (size_t i = 0; i < R * C; ++i) {
                _data[i] /= rhs;
            }
            return *this;
        }

        Matrix operator+(const Matrix &rhs) const {
            Matrix result(*this);
            result += rhs;
            return result;
        }

        Matrix operator-(const Matrix &rhs) const {
            Matrix result(*this);
            result -= rhs;
            return result;
        }

        Matrix operator*(const Matrix &rhs) const {
            Matrix result(*this);
            result *= rhs;
            return result;
        }

        Matrix operator*(T rhs) const {
            Matrix result(*this);
            result *= rhs;
            return result;
        }

        Matrix operator/(T rhs) const {
            Matrix result(*this);
            result /= rhs;
            return result;
        }

        bool operator==(const Matrix &rhs) const {
            for (size_t i = 0; i < R * C; ++i) {
                if (_data[i] != rhs._data[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const Matrix &rhs) const {
            return *this != rhs;
        }

        Matrix transpose() const {
            Matrix result;
            for (size_t r = 0; r < R; ++r) {
                for (size_t c = 0; c < C; ++c) {
                    result(c, r) = (*this)(r, c);
                }
            }
            return result;
        }

        Matrix inverse() const {
            Matrix result;
            for (size_t r = 0; r < R; ++r) {
                for (size_t c = 0; c < C; ++c) {
                    result(r, c) = (*this)(r, c);
                }
            }
            return result;
        }

    private:
        T _data[R * C];
    };

    template <typename T = float>
    using Matrix2 = Matrix<T, 2, 2>;
    using Matrix2f = Matrix2<float>;
    using Matrix2d = Matrix2<double>;

    template <typename T = float>
    using Matrix3 = Matrix<T, 3, 3>;
    using Matrix3f = Matrix3<float>;
    using Matrix3d = Matrix3<double>;

    template <typename T = float>
    using Matrix4 = Matrix<T, 4, 4>;
    using Matrix4f = Matrix4<float>;
    using Matrix4d = Matrix4<double>;
}