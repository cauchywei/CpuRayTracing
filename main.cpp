#include "src/Vector.h"
#include "src/Rect.h"
#include "src/Ray.h"
#include "src/Triangle.h"
#include "src/Sphere.h"
#include "src/Plane.h"

#include "third_party/svpng/svpng.inc"
#include "src/Scene.h"
#include "src/Texture2D.h"

#include <vector>
#include <iostream>
#include <thread>

#define MAX_REFLECTIONS 5
#define REFLECTION_RAY_EPSILON 0.006

using namespace crt;


struct LightSource {
    Vector3f position;
    Vector3f color;
};

static Vector3f rayColor(const Scene &scene,
                         const std::vector<LightSource> &lightSources,
                         const Ray &ray,
                         float tMin,
                         float tMax,
                         int depth = 0) {
    HitRecord hitRecord{};
    HitRecord shadowHitRecord{};
    Vector3f color{};
    if (scene.hit(ray, tMin, tMax, hitRecord)) {
        const auto &hitPoint = hitRecord.p;
        const auto &normal = hitRecord.normal;

        Material &material = hitRecord.material;
        color = material.getAmbient();

        for (const auto &lightSource: lightSources) {
            const auto l = (lightSource.position - hitPoint).normalize();
            const Ray &shadowRay = Ray(hitPoint, l);
            if (!scene.hit(shadowRay,
                           REFLECTION_RAY_EPSILON,
                           std::numeric_limits<float>::max(), shadowHitRecord)) {
                const auto diffuseColor = lightSource.color * normal.dot(l);

                const auto v = (ray.getOrigin() - hitPoint).normalize();
                const auto h = (l + v).normalize();
                auto specularColor =
                                     lightSource.color * std::pow(normal.dot(h),
                                                                  material.getShininess());

                if (depth < MAX_REFLECTIONS && material.getSpecular() > 0.0f) {
                    const auto r = ray.getDirection() - normal * 2 * normal.dot(ray.getDirection());
                    const auto reflectedRay = Ray(hitPoint, r);
                    const auto reflectedColor = rayColor(scene, lightSources,
                                                         reflectedRay,
                                                         REFLECTION_RAY_EPSILON,
                                                         std::numeric_limits<float>::max(),
                                                         depth + 1);
                    specularColor += lightSource.color * reflectedColor;
                }

                color += (material.getDiffuse() * diffuseColor + material.getSpecular() * specularColor);
            }
        }

        color *= hitRecord.color;

        color = color.min(Vector3f{1.0f, 1.0f, 1.0f});
    }
    return color;
}

auto loadMoonTexture() {
    const auto width = 1024;
    const auto height = 512;
    std::vector<unsigned char> data;
    auto *fp = fopen("../resource/moon-1024-512-rgb24.raw", "r");
    assert(fp);
    fseek(fp, 0, SEEK_END);
    auto size = ftell(fp);
    assert(size == width * height * 3);
    rewind(fp);
    data.resize(size);
    fread(data.data(), 1, size, fp);
    fclose(fp);

    std::vector<Vector3f> pixels;
    pixels.resize(width * height);
    for (int i = 0; i < width * height; ++i) {
        pixels[i] = Vector3f{
                static_cast<float>(data[i * 3 + 0]) / 255.0f,
                static_cast<float>(data[i * 3 + 1]) / 255.0f,
                static_cast<float>(data[i * 3 + 2]) / 255.0f
        };
    }

    return std::make_shared<Texture2D>(width, height, pixels);
}

int main() {

    auto moonTexture = loadMoonTexture();

    const float scale = 1.0f;
    SizeI outputPixelSize = {static_cast<int>(640 * scale), static_cast<int>(480 * scale)};
    SizeF cameraFrameSize = {static_cast<float>(outputPixelSize.getWidth()) * 1.0f,
                             static_cast<float>(outputPixelSize.getHeight()) * 1.0f};

    const Vector3f cameraOrigin{0.0f, 150.0f, 500.0f};
    const float focalLength = 500.0f * scale;


    std::vector<LightSource> lightSources = {
            {{150.0f * 4.0f,  500.0f * 1.5f, 1200.0f}, {1.0f, 1.0f, 1.0f}},
            {{-150.0f * 1.0f, 400.0f,        400.0f},  {0.3f, 0.3f, 0.3f}},
//            {{0.0f, 120.0f,        -80.0f},  {0.3f, 0.3f, 0.3f}},
    };

    const Vector3f cameraFrameCenter = {cameraOrigin.getX(), cameraOrigin.getY(),
                                        cameraOrigin.getZ() - focalLength};

    const auto left = -cameraFrameSize.getWidth() / 2;
    const auto right = cameraFrameSize.getWidth() / 2;
    const auto top = cameraFrameSize.getHeight() / 2;
    const auto bottom = -cameraFrameSize.getHeight() / 2;

    Scene scene;

    {
        const auto material = Material({0.01f, 0.01f, 0.01f},
                                       {0.8f, 0.2f, 0.2f},
                                       {.3f, .3f, .3f},
                                       200.0f);
        auto sphere = std::make_shared<Sphere>(Vector3f{0.0f, 50.0f, 0.0f}, 50.0f);
        sphere->setMaterial(material);
        scene.addSurface(sphere);
    }

    {
        const auto material = Material({0.3f, 0.3f, 0.3f},
                                       {0.8f, 0.8f, 0.8f},
                                       {0.0f, 0.0f, 0.0f},
                                       0.0f);
        auto sphere = std::make_shared<Sphere>(Vector3f{80.0f, 30.0f, 50.0f}, 30.0f);
        sphere->setTexture(moonTexture);
        scene.addSurface(sphere);
    }

    {
        const auto material = Material({0.01f, 0.01f, 0.01f},
                                       {0.1f, 0.5f, 0.1f},
                                       {0.0f, 0.0f, 0.0f},
                                       0.0f);
        scene.addSurface(std::make_shared<Sphere>(Vector3f{130.0f, 50.0f, -50.0f}, 50.0f, material));
    }


    {
        const Material &triangleMaterial = Material({0.01f, 0.01f, 0.01f},
                                                    {0.0f, 0.0f, 0.0f},
                                                    {0.6f, 0.6f, 0.6f},
                                                    50.0f);

        std::vector<Vector3f> vertices = {
                {-200.0f, 0.0f,   -200.0f},
                {100.0f,  0.0f,   -280.0f},
                {-200.0f, 300.0f, -200.0f},
                {100.0f,  300.0f, -280.0f},
        };
        scene.addSurface(std::make_shared<Triangle>(vertices[0], vertices[1], vertices[2], triangleMaterial));
        scene.addSurface(std::make_shared<Triangle>(vertices[1], vertices[3], vertices[2], triangleMaterial));
    }


    {
        const auto material = Material({0.3f, 0.3f, 0.3f},
                                       {0.8f, 0.8f, 0.8f},
                                       {0.0f, 0.0f, 0.0f},
                                       0.0f);
        auto moonSphere = std::make_shared<Sphere>(Vector3f{350.0f, 200.0f, -500.0f}, 200.0f,
                                                   material);
        moonSphere->setTexture(moonTexture);
        scene.addSurface(moonSphere);
    }

    {
        const Material &material = Material({},
                                            {0.2f, 0.2f, 0.2f},
                                            {0.03f, 0.03f, 0.03f},
                                            0.0f);
        scene.addSurface(std::make_shared<Plane>(Vector3f{0.0f, 1.0f, 0.0f},
                                                 Vector3f{0.0f, 0.0f, 0.0f}, material));
    }


    auto outputRGBBuffer = std::make_unique<uint8_t[]>(
            outputPixelSize.getWidth() * outputPixelSize.getHeight() * 3);

    for (int j = 0; j < outputPixelSize.getHeight(); ++j) {
        for (int i = 0; i < outputPixelSize.getWidth(); ++i) {
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

            const auto color = rayColor(scene, lightSources, ray, 0.0f, std::numeric_limits<float>::max());

            const int index = (j * outputPixelSize.getWidth() + i) * 3;
            outputRGBBuffer[index + 0] = std::min(1.0f, color.getX()) * 255;
            outputRGBBuffer[index + 1] = std::min(1.0f, color.getY()) * 255;
            outputRGBBuffer[index + 2] = std::min(1.0f, color.getZ()) * 255;
        }
    }

    FILE *fp = fopen("../out/test.png", "wb");
    assert(fp);
    svpng(fp, outputPixelSize.getWidth(), outputPixelSize.getHeight(), outputRGBBuffer.get(), 0);
    fclose(fp);

    return 0;
}
