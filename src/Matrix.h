#pragma once

#include "Vector.h"

#include <cstddef>
#include <array>

namespace crt {
    template<typename Scalar, size_t Rows, size_t Cols>
    class Matrix {
    public:

    public:
        Matrix(): _data{} {}

        Matrix(const Matrix& rhs) : _data(rhs._data) {
        }

        Matrix(Matrix&& rhs) noexcept: _data(std::move(rhs._data)) {
        }

        template<typename ...Args, typename = std::enable_if_t<std::conjunction_v<std::is_convertible<Args, Scalar>...>>>
        explicit Matrix(Args&& ... args): _data{std::forward<Args>(args)...} {
        }

        static Matrix makeIdentity() {
            static_assert(Rows == Cols, "Matrix must be square");
            Matrix<Scalar, Rows, Cols> m;
            for (size_t i = 0; i < Rows; ++i)
                m(i, i) = 1;
            return m;
        }

        Scalar& operator()(size_t r, size_t c) {
            return _data[r * Cols + c];
        }

        const Scalar& operator()(size_t r, size_t c) const {
            return _data[r * Cols + c];
        }

        Scalar& operator[](size_t i) {
            return _data[i];
        }

        const Scalar& operator[](size_t i) const {
            return _data[i];
        }

        Matrix& operator=(const Matrix& rhs) {
            _data = rhs._data;
            return *this;
        }

        Matrix& operator=(Matrix&& rhs) noexcept {
            _data = std::move(rhs._data);
            return *this;
        }

        Matrix& operator+=(const Matrix& rhs) {
            for (size_t i = 0; i < Rows * Cols; ++i) {
                _data[i] += rhs._data[i];
            }
            return *this;
        }

        Matrix& operator-=(const Matrix& rhs) {
            for (size_t i = 0; i < Rows * Cols; ++i) {
                _data[i] -= rhs._data[i];
            }
            return *this;
        }

        template<size_t RhsRows, size_t RhsCols>
        Matrix<Scalar, Rows, RhsCols>& operator*=(const Matrix<Scalar, RhsRows, RhsCols>& rhs) {
            static_assert(Cols == RhsRows, "Matrix dimensions must match");
            Matrix<Scalar, Rows, RhsCols> result;
            for (size_t r = 0; r < Rows; ++r) {
                for (size_t c = 0; c < RhsCols; ++c) {
                    result(r, c) = 0;
                    for (size_t i = 0; i < Cols; ++i) {
                        result(r, c) += (*this)(r, i) * rhs(i, c);
                    }
                }
            }
            *this = result;
            return *this;
        }

        Matrix& operator*=(Scalar rhs) {
            for (size_t i = 0; i < Rows * Cols; ++i) {
                _data[i] *= rhs;
            }
            return *this;
        }

        template<size_t RhsRows, size_t RhsCols>
        Matrix operator*(const Matrix<Scalar, RhsRows, RhsCols>& rhs) const {
            Matrix result(*this);
            result *= rhs;
            return result;
        }

        Matrix operator*(Scalar rhs) const {
            Matrix result(*this);
            result *= rhs;
            return result;
        }

        template<size_t VectorSize>
        Vector<Scalar, VectorSize> operator*(const Vector<Scalar, VectorSize>& rhs) const {
            static_assert(VectorSize == Cols, "Matrix and vector dimensions must match");
            Vector<Scalar, VectorSize> result;
            for (size_t r = 0; r < Rows; ++r) {
                result[r] = static_cast<Scalar>(0);
                for (size_t c = 0; c < Cols; ++c) {
                    result[r] += (*this)(r, c) * rhs[c];
                }
            }
            return result;
        }

        Matrix& operator/=(Scalar rhs) {
            for (size_t i = 0; i < Rows * Cols; ++i) {
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

        Matrix operator/(Scalar rhs) const {
            Matrix result(*this);
            result /= rhs;
            return result;
        }

        bool operator==(const Matrix& rhs) const {
            if (this == &rhs) {
                return true;
            }

            for (size_t i = 0; i < Rows * Cols; ++i) {
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
            Matrix<Scalar, Cols, Rows> result;
            transposeTo(result);
            *this = result;
            return *this;
        }

        void transposeTo(Matrix& result) const {
            for (size_t r = 0; r < Rows; ++r) {
                for (size_t c = 0; c < Cols; ++c) {
                    result(c, r) = (*this)(r, c);
                }
            }
        }

        Matrix& inverse() {
            Matrix<Scalar, Cols, Rows> result;
            inverseTo(result);
            *this = result;
            return *this;
        }

        void inverseTo(Matrix& result) const {
            Matrix<Scalar, Cols, Cols> cofactors;
            for (size_t r = 0; r < Rows; ++r) {
                for (size_t c = 0; c < Cols; ++c) {
                    // Calculate the cofactor matrix
                    cofactors(r, c) = cofactor(r, c);
                }
            }

            // Calculate the determinant
            Scalar det = 0;
            for (size_t c = 0; c < Cols; ++c) {
                det += (*this)(0, c) * cofactors(0, c);
            }

            // Calculate the inverse matrix
            for (size_t r = 0; r < Rows; ++r) {
                for (size_t c = 0; c < Cols; ++c) {
                    result(r, c) = cofactors(c, r) / det;
                }
            }
        }

        Matrix& identity() {
            Matrix<Scalar, Cols, Rows> result;
            identityTo(result);
            *this = result;
            return *this;
        }

        void identifyTo(Matrix& result) const {
            for (size_t r = 0; r < Rows; ++r) {
                for (size_t c = 0; c < Cols; ++c) {
                    result(r, c) = (r == c) ? 1 : 0;
                }
            }
            return result;
        }

        [[nodiscard]] bool isIdentify() const {
            static_assert(Rows == Cols, "Matrix must be square");
            for (size_t r = 0; r < Rows; ++r) {
                for (size_t c = 0; c < Cols; ++c) {
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

        Scalar determinant() const {
            static_assert(Rows == Cols, "Only square matrix can have determinant");

            if constexpr( Rows == 1) {
                return (*this)(0, 0);
            } else if constexpr(Rows == 4) {
                Scalar const _3142_3241(_data[8] * _data[13] - _data[9] * _data[12]);
                Scalar const _3143_3341(_data[8] * _data[14] - _data[10] * _data[12]);
                Scalar const _3144_3441(_data[8] * _data[15] - _data[11] * _data[12]);
                Scalar const _3243_3342(_data[9] * _data[14] - _data[10] * _data[13]);
                Scalar const _3244_3442(_data[9] * _data[15] - _data[11] * _data[13]);
                Scalar const _3344_3443(_data[10] * _data[15] - _data[11] * _data[14]);

                return _data[0] * (_data[5] * _3344_3443 - _data[6] * _3244_3442 + _data[7] * _3243_3342)
                       - _data[1] * (_data[4] * _3344_3443 - _data[6] * _3144_3441 + _data[7] * _3143_3341)
                       + _data[2] * (_data[4] * _3244_3442 - _data[5] * _3144_3441 + _data[7] * _3142_3241)
                       - _data[3] * (_data[4] * _3243_3342 - _data[5] * _3143_3341 + _data[6] * _3142_3241);
            } else {
                Scalar result = 0;
                for (size_t i = 0; i < Rows; ++i) {
                    result += (*this)(0, i) * this->cofactor(0, i);
                }
                return result;
            }
        }

        Scalar cofactor(size_t row, size_t col) const {
            static_assert(Rows == Cols, "Only square matrix can have determinant");

            if constexpr (Rows == 1) {
                return (*this)(0, 0);
            }

            Matrix<Scalar, Rows - 1, Cols - 1> subMatrix;
            for (size_t r = 0; r < Rows; ++r) {
                for (size_t c = 0; c < Cols; ++c) {
                    if (r != row && c != col) {
                        subMatrix(r, c) = (*this)(r, c);
                    }
                }
            }

            return (row + col) % 2 == 0 ? subMatrix.determinant() : -subMatrix.determinant();
        }

    private:
        std::array<Scalar, Rows * Cols> _data;
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