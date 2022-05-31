#pragma once

#include "Vector.h"

#include <utility>
#include <vector>

namespace crt {
    class Texture2D {
    public:
        Texture2D(int width, int height, const std::vector<Vector3f> &pixels);
        Texture2D(int width, int height, std::vector<Vector3f> &&pixels);

        ~Texture2D() = default;

        [[nodiscard]] int getWidth() const {
            return _width;
        }

        [[nodiscard]] int getHeight() const {
            return _height;
        }

        [[nodiscard]] const Vector3f &getPixel(int x, int y) const {
            return _pixels[y * _width + x];
        }

        [[nodiscard]] Vector3f getColor(const Vector2f &uv) const;

    private:
        // RGB888
        std::vector<Vector3f> _pixels;
        int _width;
        int _height;
    };
    using Texture2DPtr = std::shared_ptr<Texture2D>;
}