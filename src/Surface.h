#pragma once

#include "Ray.h"
#include "HitRecord.h"
#include "Texture2D.h"

#include <memory>
#include <utility>

namespace crt {

    class Surface {
    public:
        Surface(Material material, Texture2DPtr texture) : _material(std::move(material)),
                                                           _texture(std::move(texture)) {}

        explicit Surface(Material material) : _material(std::move(material)), _texture() {}

        explicit Surface(Texture2DPtr texture) : _material(), _texture(std::move(texture)) {}

        explicit Surface() : _material(), _texture() {}

        virtual ~Surface() = default;

        virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &outRecord) const = 0;

        [[nodiscard]] virtual bool hit(const Ray &ray, float tMin, float tMax) const {
            HitRecord record{};
            return hit(ray, tMin, tMax, record);
        }

        [[nodiscard]] virtual bool hit(const Ray &ray, HitRecord &outRecord) const {
            return hit(ray, 0.0f, std::numeric_limits<float>::max(), outRecord);
        }

        [[nodiscard]] virtual Vector3f getNormal(const Vector3f &p) const = 0;

        [[nodiscard]] virtual Vector2f getUV(const Vector3f &p) const = 0;

        [[nodiscard]] Vector3f getColor(const Vector3f &p) const {
            const auto uv = getUV(p);
            if (_texture) {
                return _texture->getColor(uv);
            }
            return {1.0f, 1.0f, 1.0f};
        }

        [[nodiscard]] const Material &getMaterial() const {
            return _material;
        }

        [[nodiscard]] Material &getMaterial() {
            return _material;
        }

        void setMaterial(const Material &material) {
            _material = material;
        }

        [[nodiscard]] const Texture2DPtr &getTexture() const {
            return _texture;
        }

        void setTexture(const Texture2DPtr &texture) {
            _texture = texture;
        }

//        virtual bool boundingBox(float t0, float t1, AABB& outBox) const = 0;

    private:
        Material _material;
        Texture2DPtr _texture;
    };

    using SurfacePtr = std::shared_ptr<Surface>;
}