#include "Scene.h"

namespace crt {
    bool Scene::hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const {
        auto minT = tMax;
        HitRecord tempHitRecord;
        bool hasHit = false;
        for (const auto& surface: _surfaces) {
            if (surface->hit(ray, tMin, tMax, tempHitRecord)) {
                hasHit = true;
                if (tempHitRecord.t < minT) {
                    hitRecord = tempHitRecord;
                    minT = tempHitRecord.t;
                }
            }
        }
        return hasHit;
    }

    bool Scene::hit(const Ray& ray, HitRecord& hitRecord) const {
        return hit(ray, 0.0f, std::numeric_limits<float>::max(), hitRecord);
    }

}

