#pragma once

#include <cstddef>
#include <array>

namespace crt {
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

        T determinant() const {
            static_assert(R == C, "Only square matrix can have determinant");

            if (R == 1) {
                return (*this)(0, 0);
            } else if constexpr(R == 4) {
                T const _3142_3241(_data[8] * _data[13] - _data[9] * _data[12]);
                T const _3143_3341(_data[8] * _data[14] - _data[10] * _data[12]);
                T const _3144_3441(_data[8] * _data[15] - _data[11] * _data[12]);
                T const _3243_3342(_data[9] * _data[14] - _data[10] * _data[13]);
                T const _3244_3442(_data[9] * _data[15] - _data[11] * _data[13]);
                T const _3344_3443(_data[10] * _data[15] - _data[11] * _data[14]);

                return _data[0] * (_data[5] * _3344_3443 - _data[6] * _3244_3442 + _data[7] * _3243_3342)
                       - _data[1] * (_data[4] * _3344_3443 - _data[6] * _3144_3441 + _data[7] * _3143_3341)
                       + _data[2] * (_data[4] * _3244_3442 - _data[5] * _3144_3441 + _data[7] * _3142_3241)
                       - _data[3] * (_data[4] * _3243_3342 - _data[5] * _3143_3341 + _data[6] * _3142_3241);
            } else {
                T result = 0;
                for (size_t i = 0; i < R; ++i) {
                    result += (*this)(0, i) * this->cofactor(0, i);
                }
                return result;
            }
        }

        T cofactor(size_t row, size_t col) const {
            static_assert(R == C, "Only square matrix can have determinant");

            if (R == 1) {
                return (*this)(0, 0);
            }

            Matrix<T, R - 1, C - 1> subMatrix;
            for (size_t r = 0; r < R; ++r) {
                for (size_t c = 0; c < C; ++c) {
                    if (r != row && c != col) {
                        subMatrix(r, c) = (*this)(r, c);
                    }
                }
            }

            return (row + col) % 2 == 0 ? subMatrix.determinant() : -subMatrix.determinant();
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