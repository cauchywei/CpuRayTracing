#pragma once

#include "Surface.h"

namespace crt {
    class Plane : public Surface {
    public:
        Plane(const Vector3f &normal, const Vector3f &point, const Material &material = {});

        Plane(Vector3f &&normal, Vector3f &&point, Material &&material = {});

        Vector2f getUV(const Vector3f &p) const override;

        [[nodiscard]] Vector3f getNormal(const Vector3f &) const override {
            return _normal;
        }

        [[nodiscard]] const Vector3f &getPoint() const {
            return _point;
        }

        bool intersect(const Ray &ray, float &t) const;

        bool hit(const Ray &ray, float tMin, float tMax, HitRecord &outRecord) const override;

    private:
        Vector3f _normal;
        Vector3f _point;
        Material _material;
    };

    using PlanePtr = std::shared_ptr<Plane>;
}
