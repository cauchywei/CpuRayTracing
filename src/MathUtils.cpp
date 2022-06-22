#include "MathUtils.h"

namespace crt::MathUtils {
    bool rayIntersectsTriangle(const Ray& ray,
                               const Vector3f& v0,
                               const Vector3f& v1,
                               const Vector3f& v2,
                               float& t,
                               float& u, float& v) {
        const auto& v0v1 = v1 - v0;
        const auto& v0v2 = v2 - v0;
        auto pvec = ray.getDirection().cross(v0v2);
        float det = v0v1.dot(pvec);
#if 0
        //            if (det < std::numeric_limits<decltype<det>>::epsilon()) return false;
#else
        if (almostEqual(det, 0.0f)) return false;
#endif
        float invDet = 1 / det;

        auto tvec = ray.getOrigin() - v0;
        u = tvec.dot(pvec) * invDet;
        if (u < 0 || u > 1) return false;

        auto qvec = tvec.cross(v0v1);
        v = ray.getDirection().dot(qvec) * invDet;
        if (v < 0 || u + v > 1) return false;

        t = v0v2.dot(qvec) * invDet;
        return true;
    }

    bool rayIntersectsTriangle(const Ray& ray,
                               const Vector3f& v0,
                               const Vector3f& v1,
                               const Vector3f& v2) {
        const auto& v0v1 = v1 - v0;
        const auto& v0v2 = v2 - v1;
        auto pvec = ray.getDirection().cross(v0v2);
        float det = v0v1.dot(pvec);
#if 0
        //            if (det < std::numeric_limits<decltype<det>>::epsilon()) return false;
#else
        if (almostEqual(det, 0.0f)) return false;
#endif
        return true;
    }
}

