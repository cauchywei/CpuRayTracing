#pragma once

#include "Surface.h"

namespace crt {
    class Sphere : public Surface {
    public:
        constexpr Sphere() : _center(), _radius(0.0f), _material() {}

        constexpr Sphere(const Vector3f& center, float radius, const Material& material = {}) : _center(center), _radius(radius), _material(material) {}

        constexpr Sphere(Vector3f&& center, float radius, Material&& material = {}) : _center(std::move(center)), _radius(radius), _material(std::move(material)) {}

        [[nodiscard]] constexpr const Vector3f& getCenter() const {
            return _center;
        }

        [[nodiscard]] constexpr float getRadius() const {
            return _radius;
        }


        [[nodiscard]] const Material& getMaterial() const {
            return _material;
        }

        [[nodiscard]] Material& getMaterial() {
            return _material;
        }

        void setMaterial(const Material& material) {
            _material = material;
        }

        [[nodiscard]] constexpr Vector3f getNormal(const Vector3f& p) const {
            return (p - _center).normalize();
        }

        [[nodiscard]] bool intersect(const Ray& ray) const;

        [[nodiscard]] bool intersect(const Ray& ray, float& outT) const;

        bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;

    private:
        Vector3f _center;
        float _radius;
        Material _material;
    };

    using SpherePtr = std::shared_ptr<Sphere>;
}