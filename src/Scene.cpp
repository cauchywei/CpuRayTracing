#include "Scene.h"

namespace crt {
    bool Scene::hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const {
        for (const auto& surface: _surfaces) {
            if (surface->hit(ray, tMin, tMax, hitRecord)) {
                return true;
            }
        }
        return false;
    }

    bool Scene::hit(const Ray& ray, HitRecord& hitRecord) const {
        return hit(ray, 0.0f, std::numeric_limits<float>::max(), hitRecord);
    }

}

