#pragma once

#include "Surface.h"
#include "BoundingBox.h"

namespace crt {

    class Mesh : public Surface {
    public:
        Mesh(const std::vector<Vector3f>& points,
             const std::vector<Vector3i>& triangleVertexIndices) : _points(points),
                                                                   _triangleVertexIndices(triangleVertexIndices),
                                                                   _boundingBox{} {
            for (const auto& point: _points) {
                _boundingBox.expand(point);
            }
        }

        Mesh(std::vector<Vector3f>&& points,
             std::vector<Vector3i>&& triangleVertexIndices) : _points(std::move(points)),
                                                              _triangleVertexIndices(
                                                                      std::move(triangleVertexIndices)),
                                                              _boundingBox{} {
            for (const auto& point: _points) {
                _boundingBox.expand(point);
            }
        }

        bool hit(const Ray& ray, float tMin, float tMax, HitRecord& outRecord) const override;

        Vector2f getUV(const Vector3f& p) const override {
            return crt::Vector2f();
        }

    private:
        std::vector<Vector3f> _points;
        std::vector<Vector3i> _triangleVertexIndices;
        BoundingBox<float> _boundingBox;
    };
}