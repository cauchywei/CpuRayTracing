#pragma once

#include <utility>

#include "Vector.h"

namespace crt {
    class Material {
    public:
        constexpr Material(Vector3f ambient, Vector3f diffuse, Vector3f specular, float shininess)
                : _ambient(std::move(ambient)),
                  _diffuse(std::move(diffuse)),
                  _specular(std::move(specular)),
                  _shininess(shininess) {}

        constexpr Material() : _ambient{0.01f, 0.01f, 0.01f},
                               _diffuse{0.8f, 0.8f, 0.8f},
                               _specular{0.0f, 0.0f, 0.0f},
                               _shininess{0.0f} {}


        [[nodiscard]] constexpr const Vector3f& getAmbient() const { return _ambient; }


        [[nodiscard]] constexpr const Vector3f& getDiffuse() const { return _diffuse; }

        [[nodiscard]] constexpr const Vector3f& getSpecular() const { return _specular; }

        [[nodiscard]] constexpr float getShininess() const { return _shininess; }

        [[nodiscard]] constexpr Material cloneWithShininess(float shininess) const {
            return {_ambient, _diffuse, _specular, shininess};
        }

        [[nodiscard]] constexpr Material cloneWithAmbient(Vector3f ambient) const {
            return {ambient, _diffuse, _specular, _shininess};
        }

        [[nodiscard]] constexpr Material cloneWithDiffuse(Vector3f diffuse) const {
            return {_ambient, diffuse, _specular, _shininess};
        }

    private:
        Vector3f _ambient;
        Vector3f _diffuse;
        Vector3f _specular;
//        float _emission;
        float _shininess;
    };
}