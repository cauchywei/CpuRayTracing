#include "Triangle.h"

constexpr bool crt::Triangle::intersect(const crt::Ray& ray, float& t, float& u, float& v) const {
    const auto& v0v1 = _vertices[1] - _vertices[0];
    const auto& v0v2 = _vertices[2] - _vertices[0];
    auto pvec = ray.getDirection().cross(v0v2);
    float det = v0v1.dot(pvec);
#if 0
    //            if (det < std::numeric_limits<decltype<det>>::epsilon()) return false;
#else
    if (almostEqual(det, 0.0f)) return false;
#endif
    float invDet = 1 / det;

    auto tvec = ray.getOrigin() - _vertices[0];
    u = tvec.dot(pvec) * invDet;
    if (u < 0 || u > 1) return false;

    auto qvec = tvec.cross(v0v1);
    v = ray.getDirection().dot(qvec) * invDet;
    if (v < 0 || u + v > 1) return false;

    t = v0v2.dot(qvec) * invDet;
    return true;
}
