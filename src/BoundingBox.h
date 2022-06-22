#pragma once

#include "Ray.h"

namespace crt {

    template<typename Scalar>
    class BoundingBox {
    public:
        BoundingBox();

        BoundingBox(const Vector3<Scalar>& min, const Vector3<Scalar>& max) : _min(min), _max(max) {}

        BoundingBox(Vector3f&& min, Vector3f&& max) : _min(std::move(min)), _max(std::move(max)) {}

        [[nodiscard]] bool hit(const Ray& ray, float tMin, float tMax) const;

        [[nodiscard]] Vector3f getMin() const { return _min; }

        [[nodiscard]] Vector3f getMax() const { return _max; }

        void expand(const Vector3<Scalar>& point);

    private:
        Vector3f _min;
        Vector3f _max;
    };

    template<typename Scalar>
    BoundingBox<Scalar>::BoundingBox() {
        _min = Vector3<Scalar>{std::numeric_limits<Scalar>::max(),
                               std::numeric_limits<Scalar>::max(),
                               std::numeric_limits<Scalar>::max()};
        _max = Vector3<Scalar>{std::numeric_limits<Scalar>::lowest(),
                               std::numeric_limits<Scalar>::lowest(),
                               std::numeric_limits<Scalar>::lowest()};
    }

    template<typename Scalar>
    bool BoundingBox<Scalar>::hit(const Ray& ray, float tMin, float tMax) const {
        for (int a = 0; a < 3; a++) {
            const auto invD = 1.0f / ray.getDirection()[a];
            auto t0 = (_min[a] - ray.getOrigin()[a]) * invD;
            auto t1 = (_max[a] - ray.getOrigin()[a]) * invD;
            if (invD < 0.0f) {
                std::swap(t0, t1);
            }
            tMin = t0 > tMin ? t0 : tMin;
            tMax = t1 < tMax ? t1 : tMax;
            if (tMax <= tMin) {
                return false;
            }
        }
        return true;
    }

    template<typename Scalar>
    void BoundingBox<Scalar>::expand(const Vector3<Scalar>& point) {
        _min = Vector3<Scalar>{
                std::min(_min.getX(), point.getX()),
                std::min(_min.getY(), point.getY()),
                std::min(_min.getZ(), point.getZ())
        };
        _max = Vector3<Scalar>{
                std::max(_max.getX(), point.getX()),
                std::max(_max.getY(), point.getY()),
                std::max(_max.getZ(), point.getZ())
        };
    }
}
