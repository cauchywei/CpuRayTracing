#pragma once

#include "Ray.h"
#include "HitRecord.h"

#include <memory>

namespace crt {

    class Surface {
    public:
        virtual ~Surface() = default;

        virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& outRecord) const = 0;

        [[nodiscard]] virtual bool hit(const Ray& ray, float tMin, float tMax) const {
            HitRecord record{};
            return hit(ray, tMin, tMax, record);
        }

        [[nodiscard]] virtual bool hit(const Ray& ray, HitRecord& outRecord) const {
            return hit(ray, 0.0f, std::numeric_limits<float>::max(), outRecord);
        }

//        virtual bool boundingBox(float t0, float t1, AABB& outBox) const = 0;
    };

    using SurfacePtr = std::shared_ptr<Surface>;
}