#pragma once

#include "Ray.h"
#include "Material.h"

namespace crt {
    struct HitRecord {
    public:
        float t;
        float u;
        float v;
        Vector3f p;
        Vector3f normal;
        Material material;
        Vector3f color;
    };
}