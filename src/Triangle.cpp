#include "Triangle.h"

#include "MathUtils.h"

namespace crt {

    bool Triangle::hit(const Ray& ray, float tMin, float tMax, HitRecord& outRecord) const {
        float t, u, v;
        if (MathUtils::rayIntersectsTriangle(ray, _vertices[0], _vertices[1], _vertices[2], t, u, v) && t >= tMin &&
            t <= tMax) {
            outRecord.t = t;
            outRecord.u = u;
            outRecord.v = v;
            outRecord.p = ray.getPoint(t);
            outRecord.normal = (_vertices[1] - _vertices[0]).cross(_vertices[2] - _vertices[0]).normalize();
            outRecord.material = getMaterial();
            outRecord.color = getColor(outRecord.p);
            return true;
        }
        return false;
    }

    Vector2f Triangle::getUV(const Vector3f& p) const {
        // calculate uv base on barycentric coordinates
        const auto& v0v1 = _vertices[1] - _vertices[0];
        const auto& v0v2 = _vertices[2] - _vertices[0];
        auto pvec = p - _vertices[0];
        float u = v0v1.dot(pvec);
        float v = v0v2.dot(pvec);
        return {u, v};
    }
}
