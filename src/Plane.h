#pragma once

#include "Surface.h"

namespace crt {
    class Plane : public Surface {
    public:
        constexpr Plane(const Vector3f& normal, const Vector3f& point) : _normal(normal), _point(point) {}

        constexpr Plane(Vector3f&& normal, Vector3f&& point) : _normal(std::move(normal)), _point(std::move(point)) {}

        [[nodiscard]] const Vector3f& getNormal() const {
            return _normal;
        }

        [[nodiscard]] const Vector3f& getPoint() const {
            return _point;
        }

        bool intersect(const Ray& ray, float& t) const {
            float denom = _normal.dot(ray.getDirection());
            if (denom < 0) {
                t = (_point - ray.getOrigin()).dot(_normal) / denom;
                return t >= 0;
            }
            return false;
        }

        bool hit(const Ray& ray, float tMin, float tMax, HitRecord& outRecord) const override {
            float t;
            if (intersect(ray, t) && t >= tMin && t <= tMax) {
                outRecord.t = t;
                outRecord.p = ray.getPoint(t);
                outRecord.normal = _normal;
                return true;
            }
            return false;
        }

    private:
        Vector3f _normal;
        Vector3f _point;
    };

    using PlanePtr = std::shared_ptr<Plane>;
}
