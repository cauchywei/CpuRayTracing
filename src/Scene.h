#pragma once

#include "Surface.h"

#include <vector>

namespace crt {
    class Scene {
    public:

        Scene() = default;

        ~Scene() = default;

        void addSurface(const SurfacePtr& surface) {
            _surfaces.push_back(surface);
        }

        void removeSurface(const SurfacePtr& surface) {
            _surfaces.erase(std::remove(_surfaces.begin(), _surfaces.end(), surface), _surfaces.end());
        }

        void clear() {
            _surfaces.clear();
        }

        [[nodiscard]] const std::vector<SurfacePtr>& getSurface() const {
            return _surfaces;
        }

        bool hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const;

        bool hit(const Ray& ray, HitRecord& hitRecord) const;

    private:
        std::vector<SurfacePtr> _surfaces;
    };
}