#pragma once

#include "Ray.h"

namespace crt::MathUtils {

    bool rayIntersectsTriangle(const Ray& ray,
                               const Vector3f& v0,
                               const Vector3f& v1,
                               const Vector3f& v2,
                               float& t,
                               float& u,
                               float& v);

    bool rayIntersectsTriangle(const Ray& ray,
                               const Vector3f& v0,
                               const Vector3f& v1,
                               const Vector3f& v2);
};
