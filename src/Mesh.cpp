#include "Mesh.h"

#include "MathUtils.h"

namespace crt {
    bool Mesh::hit(const Ray& ray, float tMin, float tMax, HitRecord& outRecord) const {
        if (!_boundingBox.hit(ray, tMin, tMax)) {
            return false;
        }

        bool hit = false;
        float closestT = tMax;
        for (const auto& triangleVertexIndices: _triangleVertexIndices) {
            const Vector3f& v0 = _points[triangleVertexIndices[0]];
            const Vector3f& v1 = _points[triangleVertexIndices[1]];
            const Vector3f& v2 = _points[triangleVertexIndices[2]];
            float t, u, v;
            if (MathUtils::rayIntersectsTriangle(ray, v0, v1, v2, t, u, v)) {
                if (t > tMin && t < closestT) {
                    closestT = t;
                    outRecord.t = t;
                    outRecord.p = ray.getPoint(t);
                    outRecord.normal = (v1 - v0).cross(v2 - v0).normalize();
                    outRecord.material = getMaterial();
                    hit = true;
                }
            }
        }
        return hit;
    }
}

