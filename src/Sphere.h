#pragma once

#include "Surface.h"
#include "Texture2D.h"

namespace crt {
    class Sphere : public Surface {
    public:
        Sphere() : _center(), _radius(0.0f) {}

        Sphere(const Vector3f &center, float radius, const Material &material = {}) : Surface(material),
                                                                                      _center(center),
                                                                                      _radius(radius) {}

        Sphere(Vector3f &&center, float radius, Material &&material = {}) : Surface(std::move(material)),
                                                                            _center(std::move(center)),
                                                                            _radius(radius) {}

        [[nodiscard]] constexpr const Vector3f &getCenter() const {
            return _center;
        }

        [[nodiscard]] constexpr float getRadius() const {
            return _radius;
        }

        [[nodiscard]] Vector2f getUV(const Vector3f &p) const override;


        [[nodiscard]] Vector3f getNormal(const Vector3f &p) const override {
            return (p - _center).normalize();
        }

        [[nodiscard]] bool intersect(const Ray &ray) const;

        [[nodiscard]] bool intersect(const Ray &ray, float &outT) const;

        bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const override;

    private:

    private:
        Vector3f _center;
        float _radius;
    };

    using SpherePtr = std::shared_ptr<Sphere>;
}