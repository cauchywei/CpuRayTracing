#pragma once

#include "Vector.h"
#include "Ray.h"

#include "Surface.h"

namespace crt {
    class Triangle : public Surface {
    public:
        Triangle(const Vector3f &v0, const Vector3f &v1, const Vector3f &v2, const Material &material = {})
                : Surface(material), _vertices{v0, v1, v2} {}

        Triangle(Vector3f &&v0, Vector3f &&v1, Vector3f &&v2, Material &&material = {}) : _vertices{
                std::move(v0),
                std::move(v1),
                std::move(v2)} {}

        constexpr const Vector3f &operator[](int i) const { return _vertices[i]; }

        constexpr Vector3f &operator[](int i) { return _vertices[i]; }

        [[nodiscard]] constexpr const Vector3f &v0() const { return _vertices[0]; }

        [[nodiscard]] constexpr Vector3f &v0() { return _vertices[0]; }

        [[nodiscard]] constexpr const Vector3f &v1() const { return _vertices[0]; }

        [[nodiscard]] constexpr Vector3f &v1() { return _vertices[0]; }


        [[nodiscard]] Vector3f getNormal(const Vector3f &p) const override {
            // TODO: interpolate normal
            return (_vertices[1] - _vertices[0]).cross(_vertices[2] - _vertices[0]).normalize();
        }

        Vector2f getUV(const Vector3f &p) const override;

        constexpr bool operator==(const Triangle &other) const {
            return _vertices[0] == other._vertices[0]
                   && _vertices[1] == other._vertices[1]
                   && _vertices[2] == other._vertices[2];
        }

        constexpr bool operator!=(const Triangle &other) const {
            return !(*this == other);
        }

        bool intersect(const Ray &ray, float &t, float &u, float &v) const;

        bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const override;

    private:
        std::array<Vector3f, 3> _vertices;
    };

    using TrianglePtr = std::shared_ptr<Triangle>;
}