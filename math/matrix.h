#pragma once

#include <cstddef>
#include <array>

namespace cry::math {
    template<typename T, size_t R, size_t C>
    class Matrix {
    public:
        Matrix() {
            std::fill(std::begin(_data), std::end(_data), 0);
        }

        template<typename ...Args, typename = std::enable_if_t<std::conjunction_v<std::is_convertible<Args, T>...>>>
        explicit Matrix(Args&& ... args): _data{std::forward<Args>(args)...} {
        }

        Matrix(const Matrix& rhs) : _data(rhs._data) {
        }

        Matrix(Matrix&& rhs) noexcept: _data(std::move(rhs._data)) {
        }

        Matrix& operator=(const Matrix& rhs) {
            _data = rhs._data;
            return *this;
        }

        Matrix& operator=(Matrix&& rhs) noexcept {
            _data = std::move(rhs._data);
            return *this;
        }

        static Matrix MakeIdentity() {
            static_assert(R == C, "Matrix must be square");
            Matrix<T, R, C> m;
            for (size_t i = 0; i < R; ++i)
                m(i, i) = 1;
            return m;
        }

        T& operator()(size_t r, size_t c) {
            return _data[r * C + c];
        }

        const T& operator()(size_t r, size_t c) const {
            return _data[r * C + c];
        }

        T& operator[](size_t i) {
            return _data[i];
        }

        const T& operator[](size_t i) const {
            return _data[i];
        }

        Matrix& operator+=(const Matrix& rhs) {
            for (size_t i = 0; i < R * C; ++i) {
                _data[i] += rhs._data[i];
            }
            return *this;
        }

        Matrix& operator-=(const Matrix& rhs) {
            for (size_t i = 0; i < R * C; ++i) {
                _data[i] -= rhs._data[i];
            }
            return *this;
        }

        Matrix& operator*=(const Matrix& rhs) {
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

        Matrix& operator*=(T rhs) {
            for (size_t i = 0; i < R * C; ++i) {
                _data[i] *= rhs;
            }
            return *this;
        }

        Matrix& operator/=(T rhs) {
            for (size_t i = 0; i < R * C; ++i) {
                _data[i] /= rhs;
            }
            return *this;
        }

        Matrix operator+(const Matrix& rhs) const {
            Matrix result(*this);
            result += rhs;
            return result;
        }

        Matrix operator-(const Matrix& rhs) const {
            Matrix result(*this);
            result -= rhs;
            return result;
        }

        Matrix operator*(const Matrix& rhs) const {
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

        bool operator==(const Matrix& rhs) const {
            if (this == &rhs) {
                return true;
            }

            for (size_t i = 0; i < R * C; ++i) {
                if (_data[i] != rhs._data[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const Matrix& rhs) const {
            return *this != rhs;
        }

        Matrix& transpose() {
            Matrix<T, C, R> result;
            transposeTo(result);
            *this = result;
            return *this;
        }

        void transposeTo(Matrix& result) const {
            for (size_t r = 0; r < R; ++r) {
                for (size_t c = 0; c < C; ++c) {
                    result(c, r) = (*this)(r, c);
                }
            }
        }

        Matrix& inverse() {
            Matrix<T, C, R> result;
            inverseTo(result);
            *this = result;
            return *this;
        }

        void inverseTo(Matrix& result) const {
            for (size_t r = 0; r < R; ++r) {
                for (size_t c = 0; c < C; ++c) {
                    result(r, c) = this->operator()(r, c);
                }
            }
        }

        Matrix& identity() {
            Matrix<T, C, R> result;
            identityTo(result);
            *this = result;
            return *this;
        }

        void identifyTo(Matrix& result) const {
            for (size_t r = 0; r < R; ++r) {
                for (size_t c = 0; c < C; ++c) {
                    result(r, c) = (r == c) ? 1 : 0;
                }
            }
            return result;
        }

        [[nodiscard]] bool isIdentify() const {
            for (size_t r = 0; r < R; ++r) {
                for (size_t c = 0; c < C; ++c) {
                    if (r == c) {
                        if ((*this)(r, c) != 1) {
                            return false;
                        }
                    } else {
                        if ((*this)(r, c) != 0) {
                            return false;
                        }
                    }
                }
            }
            return true;
        }

    private:
        std::array<T, R * C> _data;
    };

    template<typename T = float>
    using Matrix2 = Matrix<T, 2, 2>;
    using Matrix2f = Matrix2<float>;
    using Matrix2d = Matrix2<double>;

    template<typename T = float>
    using Matrix3 = Matrix<T, 3, 3>;
    using Matrix3f = Matrix3<float>;
    using Matrix3d = Matrix3<double>;

    template<typename T = float>
    using Matrix4 = Matrix<T, 4, 4>;
    using Matrix4f = Matrix4<float>;
    using Matrix4d = Matrix4<double>;
}