#pragma once

#include "Vector.h"
#include "Ray.h"

namespace crt {
    class Sphere {
    public:
        constexpr Sphere(const Vector3f& center, float radius) : _center(center), _radius(radius) {}

        constexpr Sphere(Vector3f&& center, float radius) : _center(std::move(center)), _radius(radius) {}

        [[nodiscard]] constexpr const Vector3f& getCenter() const {
            return _center;
        }

        [[nodiscard]] constexpr float getRadius() const {
            return _radius;
        }
        
        [[nodiscard]] bool intersect(const Ray& ray) const;

        [[nodiscard]] bool intersect(const Ray& ray, float& outDistance) const;

    private:
        Vector3f _center;
        float _radius;
    };
}