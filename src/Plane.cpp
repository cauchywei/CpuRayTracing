#include "Plane.h"

namespace crt {
    Plane::Plane(const Vector3f &normal,
                 const Vector3f &point,
                 const Material &material) : Surface(material),
                                             _normal(normal),
                                             _point(point),
                                             _material(
                                                     material) {}

    Plane::Plane(Vector3f &&normal, Vector3f &&point, Material &&material) : Surface(std::move(material)),
                                                                             _normal(std::move(normal)),
                                                                             _point(std::move(point)) {}

    bool Plane::intersect(const Ray &ray, float &t) const {
        float denom = _normal.dot(ray.getDirection());
        if (denom < 0) {
            t = (_point - ray.getOrigin()).dot(_normal) / denom;
            return t >= 0;
        }
        return false;
    }

    bool Plane::hit(const Ray &ray, float tMin, float tMax, HitRecord &outRecord) const {
        float t;
        if (intersect(ray, t) && t >= tMin && t <= tMax) {
            outRecord.t = t;
            outRecord.p = ray.getPoint(t);
            outRecord.normal = _normal;
            outRecord.material = getMaterial();
            outRecord.color = getColor(outRecord.p);
            return true;
        }
        return false;
    }

    Vector2f Plane::getUV(const Vector3f &p) const {
        // TODO: implement
        return crt::Vector2f();
    }

}
