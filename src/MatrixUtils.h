#include "Matrix.h"

namespace crt::MatrixUtils {
    template<typename Scalar>
    static inline Matrix4<Scalar> makeViewportTransform(const Scalar& width, const Scalar& height) {
        constexpr auto kZero = static_cast<Scalar>(0);
        constexpr auto kOne = static_cast<Scalar>(1);
        constexpr auto kTwo = static_cast<Scalar>(2);
        return Matrix4<Scalar>{
                width / kTwo, kZero, kZero, (width - kOne) / kTwo,
                kZero, height / kTwo, kZero, (height - kOne) / kTwo,
                kZero, kZero, kOne, kZero,
                kZero, kZero, kZero, kOne,
        };
    }

    template<typename Scalar>
    static inline Matrix4<Scalar> makeOrthographicProjectionTransform(const Scalar& width,
                                                                      const Scalar& height,
                                                                      const Scalar& near,
                                                                      const Scalar& far) {
        constexpr auto kZero = static_cast<Scalar>(0);
        constexpr auto kOne = static_cast<Scalar>(1);
        constexpr auto kTwo = static_cast<Scalar>(2);
        return Matrix4<Scalar>{
                kTwo / width, kZero, kZero, -kOne,
                kZero, kTwo / height, kZero, -kOne,
                kZero, kZero, kTwo / (far - near), -(far + near) / (far - near),
                kZero, kZero, kZero, kOne,
        };
    }

    template<typename Scalar>
    static inline Matrix4<Scalar> makeCameraToWorldTransform(const Vector3<Scalar>& eye,
                                                             const Vector3<Scalar>& center,
                                                             const Vector3<Scalar>& up) {
        constexpr auto kZero = static_cast<Scalar>(0);
        constexpr auto kOne = static_cast<Scalar>(1);
        constexpr auto kTwo = static_cast<Scalar>(2);
        const auto z = (eye - center).normalize();
        const auto x = up.cross(z).normalize();
        const auto y = z.cross(x);
        return Matrix4<Scalar>{
                x[0], y[0], z[0], eye[0],
                x[1], y[1], z[1], eye[1],
                x[2], y[2], z[2], eye[2],
                kZero, kZero, kZero, kOne
        };
    }

    template<typename Scalar>
    static inline Matrix4<Scalar> makeWorldToCameraTransform(const Vector<Scalar, 3>& eye,
                                                             const Vector<Scalar, 3>& center,
                                                             const Vector<Scalar, 3>& up) {
        constexpr auto kZero = static_cast<Scalar>(0);
        constexpr auto kOne = static_cast<Scalar>(1);

        const auto z = (eye - center).normalize();
        const auto x = up.cross(z).normalize();
        const auto y = z.cross(x);

        return Matrix4<Scalar>{
                x[0], x[1], x[2], -x.dot(eye),
                y[0], y[1], y[2], -y.dot(eye),
                z[0], z[1], z[2], -z.dot(eye),
                kZero, kZero, kZero, kOne,
        };
    }

    template<typename Scalar>
    static inline Matrix4<Scalar> makePerspectiveProjectionTransform(const Scalar& left,
                                                                     const Scalar& right,
                                                                     const Scalar& bottom,
                                                                     const Scalar& top,
                                                                     const Scalar& near,
                                                                     const Scalar& far) {
        constexpr auto kZero = static_cast<Scalar>(0);
        constexpr auto kOne = static_cast<Scalar>(1);
        constexpr auto kTwo = static_cast<Scalar>(2);

        return Matrix4<Scalar>{
                kTwo * near / (right - left), kZero, (left + right) / (left - right), kZero,
                kZero, kTwo * near / (top - bottom), (bottom + top) / (bottom - top), kZero,
                kZero, kZero, (far + near) / (near - far), -(kTwo * far * near) / (near - far),
                kZero, kZero, kOne, kZero,
        };
    }

    template<typename Scalar>
    static inline void getViewPlaneBounds(const Scalar& fov,
                                          const Scalar& aspect,
                                          const Scalar& near,
                                          const Scalar& far,
                                          Scalar& left,
                                          Scalar& right,
                                          Scalar& bottom,
                                          Scalar& top) {

        constexpr auto kTwo = static_cast<Scalar>(2);
        const auto scale = static_cast<Scalar>(std::tan(fov / kTwo)) * std::abs(near);

        right = aspect * scale;
        left = -right;
        top = scale;
        bottom = -top;
    }

    template<typename Scalar>
    static inline Matrix4<Scalar> makePerspectiveProjectionTransform(const Scalar& fov,
                                                                     const Scalar& aspect,
                                                                     const Scalar& near,
                                                                     const Scalar& far) {
        Scalar left, right, bottom, top;
        getViewPlaneBounds(fov, aspect, near, far, left, right, bottom, top);
        return makePerspectiveProjectionTransform(left, right, bottom, top, near, far);
    }


    template<typename Scalar>
    static Matrix4<Scalar> scale(const Vector3<Scalar>& scale) {
        return scale(scale[0], scale[1], scale[2]);
    }

    template<typename Scalar>
    static Matrix4<Scalar> scale(const Scalar& scaleX, const Scalar& scaleY, const Scalar& scaleZ) {
        constexpr auto kZero = static_cast<Scalar>(0);
        constexpr auto kOne = static_cast<Scalar>(1);
        return Matrix4<Scalar>{
                scaleX, kZero, kZero, kZero,
                kZero, scaleY, kZero, kZero,
                kZero, kZero, scaleZ, kZero,
                kZero, kZero, kZero, kOne
        };
    }

    template<typename Scalar>
    static Matrix4<Scalar> scale(Scalar scale) {
        constexpr auto kZero = static_cast<Scalar>(0);
        constexpr auto kOne = static_cast<Scalar>(1);
        return Matrix4<Scalar>{
                scale, kZero, kZero, kZero,
                kZero, scale, kZero, kZero,
                kZero, kZero, scale, kZero,
                kZero, kZero, kZero, kOne
        };
    }

    template<typename Scalar>
    static Matrix4<Scalar> translate(const Vector3<Scalar>& translation) {
        constexpr auto kZero = static_cast<Scalar>(0);
        constexpr auto kOne = static_cast<Scalar>(1);
        return Matrix4<Scalar>{
                kOne, kZero, kZero, translation.getX(),
                kZero, kOne, kZero, translation.getY(),
                kZero, kZero, kOne, translation.getZ(),
                kZero, kZero, kZero, kOne
        };
    }

    template<typename Scalar>
    static Matrix4<Scalar> translate(Scalar x, Scalar y, Scalar z) {
        constexpr auto kZero = static_cast<Scalar>(0);
        constexpr auto kOne = static_cast<Scalar>(1);
        return Matrix4<Scalar>{
                kOne, kZero, kZero, x,
                kZero, kOne, kZero, y,
                kZero, kZero, kOne, z,
                kZero, kZero, kZero, kOne
        };
    }

    template<typename Scalar>
    static Matrix4<Scalar> rotateByX(Scalar angle) {
        constexpr auto kZero = static_cast<Scalar>(0);
        constexpr auto kOne = static_cast<Scalar>(1);
        Scalar const cos = std::cos(angle);
        Scalar const sin = std::sin(angle);
        return Matrix4<Scalar>{
                kOne, kZero, kZero, kZero,
                kZero, cos, -sin, kZero,
                kZero, sin, cos, kZero,
                kZero, kZero, kZero, kOne
        };
    }

    template<typename Scalar>
    static Matrix4<Scalar> rotateByY(float angle) {
        constexpr auto kZero = static_cast<Scalar>(0);
        constexpr auto kOne = static_cast<Scalar>(1);
        Scalar const cos = std::cos(angle);
        Scalar const sin = std::sin(angle);
        return Matrix4<Scalar>{
                cos, kZero, sin, kZero,
                kZero, kOne, kZero, kZero,
                -sin, kZero, cos, kZero,
                kZero, kZero, kZero, kOne,
        };
    }

    template<typename Scalar>
    static Matrix4<Scalar> rotateByZ(Scalar angle) {
        constexpr auto kZero = static_cast<Scalar>(0);
        constexpr auto kOne = static_cast<Scalar>(1);
        Scalar const cos = std::cos(angle);
        Scalar const sin = std::sin(angle);
        return Matrix4<Scalar>{
                cos, -sin, kZero, kZero,
                sin, cos, kZero, kZero,
                kZero, kZero, kOne, kZero,
                kZero, kZero, kZero, kOne,
        };
    }

}