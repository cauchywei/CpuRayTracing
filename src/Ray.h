#pragma once

#include "Vector.h"

namespace crt {
    class Ray {
    public:
        constexpr Ray(const Vector3f& origin, const Vector3f& direction) : _origin(origin), _direction(direction) {}

        constexpr Ray(Vector3f&& origin, Vector3f&& direction) : _origin(std::move(origin)),
                                                                 _direction(std::move(direction)) {}

        [[nodiscard]] constexpr const Vector3f& getOrigin() const {
            return _origin;
        }

        [[nodiscard]] constexpr const Vector3f& getDirection() const {
            return _direction;
        }

    private:
        Vector3f _origin;
        Vector3f _direction;
    };
}