#include "Sphere.h"

namespace crt {
    bool Sphere::intersect(const crt::Ray& ray) const {
        Vector3f oc = ray.getOrigin() - _center;
        float a = ray.getDirection().dot(ray.getDirection());
        float b = oc.dot(ray.getDirection());
        float c = oc.dot(oc) - _radius * _radius;
        float discriminant = b * b - a * c;
        return discriminant > 0;
    }

    bool Sphere::intersect(const Ray& ray, float& outDistance) const {
        const Vector3f& oc = ray.getOrigin() - _center;
        const float a = ray.getDirection().dot(ray.getDirection());
        const float b = 2.0f * oc.dot(ray.getDirection());
        const float c = oc.dot(oc) - _radius * _radius;
        const float discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0.0f) {
            return false;
        }
        if (discriminant == 0.0f) {
            outDistance = -b / (2.0f * a);
            return true;
        } else {
            const float t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);
            const float t2 = (-b + std::sqrt(discriminant)) / (2.0f * a);
            if (t1 < 0.0f && t2 < 0.0f) {
                return false;
            }
            if (t1 < 0.0f) {
                outDistance = t2;
            } else if (t2 < 0.0f) {
                outDistance = t1;
            } else {
                outDistance = std::min(t1, t2);
            }
            return true;
        }
    }
}

