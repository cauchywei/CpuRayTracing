#include "Texture2D.h"

crt::Texture2D::Texture2D(int width, int height, const std::vector<Vector3f> &pixels) : _width(width), _height(height),
                                                                                        _pixels(pixels) {
    assert(width > 0);
    assert(height > 0);
    assert(pixels.size() == width * height);
}

crt::Texture2D::Texture2D(int width, int height, std::vector<Vector3f> &&pixels) : _width(width), _height(height),
                                                                                   _pixels(std::move(pixels)) {
    assert(width > 0);
    assert(height > 0);
    assert(pixels.size() == width * height);
}


template<class T>
static T lerp(T x0, T x1, float frac) {
    return x0 + (x1 - x0) * frac;
}

crt::Vector3f crt::Texture2D::getColor(const crt::Vector2f &uv) const {
    const auto x = uv.getX() * static_cast<float>(_width);
    const auto y = uv.getY() * static_cast<float>(_height);
    const auto xFloor = std::floor(x);
    const auto yFloor = std::floor(y);
    const auto xCeil = std::ceil(x);
    const auto yCeil = std::ceil(y);
    const auto xFrac = x - xFloor;
    const auto yFrac = y - yFloor;
    const auto x0 = static_cast<int>(xFloor);
    const auto y0 = static_cast<int>(yFloor);
    const auto x1 = static_cast<int>(xCeil);
    const auto y1 = static_cast<int>(yCeil);
    const auto c00 = _pixels[y0 * _width + x0];
    const auto c01 = _pixels[y0 * _width + x1];
    const auto c10 = _pixels[y1 * _width + x0];
    const auto c11 = _pixels[y1 * _width + x1];
    const auto c0 = lerp(c00, c01, xFrac);
    const auto c1 = lerp(c10, c11, xFrac);
    return lerp(c0, c1, yFrac);
}
