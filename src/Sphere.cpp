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

    bool Sphere::intersect(const Ray& ray, float& outT) const {
        const auto& oc = ray.getOrigin() - _center;
        const float a = ray.getDirection().dot(ray.getDirection());
        const float b = 2.0f * oc.dot(ray.getDirection());
        const float c = oc.dot(oc) - _radius * _radius;
        const float discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0.0f) {
            return false;
        }
        if (discriminant == 0.0f) {
            outT = -b / (2.0f * a);
            return true;
        } else {
            const float t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);
            const float t2 = (-b + std::sqrt(discriminant)) / (2.0f * a);
            if (t1 < 0.0f && t2 < 0.0f) {
                return false;
            }
            if (t1 < 0.0f) {
                outT = t2;
            } else if (t2 < 0.0f) {
                outT = t1;
            } else {
                outT = std::min(t1, t2);
            }
            return true;
        }
    }

    bool Sphere::hit(const Ray& ray, float tMin, float tMax, HitRecord& outRecord) const {
        float t;
        if (intersect(ray, t) && t >= tMin && t <= tMax) {
            outRecord.t = t;
            outRecord.p = ray.getPoint(t);
            outRecord.normal = (outRecord.p - _center) / _radius;
            outRecord.material = _material;
            return true;
        }
        return false;
    }

}

