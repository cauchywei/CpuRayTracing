#include "src/Vector.h"
#include "src/Rect.h"
#include "src/Ray.h"

#include <iostream>

using namespace crt;

struct Plane {
    Vector3f normal;
    Vector3f point;
};

int main() {

    const Vector3f cameraOrigin{0.0f, 0.0f, 0.0f};
    const float focalLength = 10.0f;
    SizeF cameraFrameSize = {640.0f, 480.0f};
    SizeI outputPixelSize = {640, 480};


    const Vector3f cameraFrameCenter = {cameraOrigin.getX(), cameraOrigin.getY(), cameraOrigin.getZ() - focalLength};

    const auto left = -cameraFrameSize.getWidth() / 2;
    const auto right = cameraFrameSize.getWidth() / 2;
    const auto top = cameraFrameSize.getHeight() / 2;
    const auto bottom = -cameraFrameSize.getHeight() / 2;

    for (int i = 0; i < outputPixelSize.getWidth(); ++i) {
        for (int j = 0; j < outputPixelSize.getHeight(); ++j) {
            const auto dx = left + (right - left) * (static_cast<float>(i) + 0.5f) /
                                   static_cast<float>(outputPixelSize.getWidth());
            const auto dy = top - (top - bottom) * (static_cast<float>(j) + 0.5f) /
                                  static_cast<float>(outputPixelSize.getHeight());
            const Vector3f point = {
                    cameraFrameCenter.getX() + dx,
                    cameraFrameCenter.getY() + dy,
                    cameraFrameCenter.getZ()
            };
            const auto rayDirection = (point - cameraOrigin).normalize();
            const auto rayOrigin = cameraOrigin;
            const auto ray = Ray{rayOrigin, rayDirection};
            // TODO

        }
    }

    return 0;
}
