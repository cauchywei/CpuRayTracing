#include "src/Vector.h"
#include "src/Rect.h"
#include "src/Ray.h"
#include "src/Triangle.h"
#include "src/Sphere.h"
#include "src/Plane.h"
#include "src/Scene.h"
#include "src/Texture2D.h"
#include "src/Matrix.h"
#include "src/MatrixUtils.h"

#include "third_party/svpng/svpng.inc"
#include "src/Mesh.h"

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

static Vector3f rayColor(const Scene& scene,
                         const std::vector<LightSource>& lightSources,
                         const Ray& ray,
                         const float tMin,
                         const float tMax,
                         const int depth = 0) {
    HitRecord hitRecord{};
    HitRecord shadowHitRecord{};
    Vector3f color{};
    if (scene.hit(ray, tMin, tMax, hitRecord)) {
        const auto& hitPoint = hitRecord.p;
        const auto& normal = hitRecord.normal;

        Material& material = hitRecord.material;
        color = material.getAmbient();

        for (const auto& lightSource: lightSources) {
            const auto l = (lightSource.position - hitPoint).normalize();
            const Ray& shadowRay = Ray(hitPoint, l);
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

        color *= color.dot(hitRecord.color);

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

std::unique_ptr<Mesh> loadDragonMesh() {
    auto *fp = fopen("../resource/dragon_vrip_res4.ply", "r");
    assert(fp);

    int vertexCount = 0;
    int faceCount = 0;
    fscanf(fp, "ply\n");
    fscanf(fp, "format ascii 1.0\n");
    fscanf(fp, "comment generated by ply_writer\n");
    fscanf(fp, "element vertex %d\n", &vertexCount);
    fscanf(fp, "property float x\n");
    fscanf(fp, "property float y\n");
    fscanf(fp, "property float z\n");
    fscanf(fp, "element face %d\n", &faceCount);
    fscanf(fp, "property list uchar int vertex_indices\n");
    fscanf(fp, "end_header\n");

    // Transformation matrix
    Matrix4f transform = Matrix4f::makeIdentity();
    transform = MatrixUtils::scale(1000.0f, 1000.0f, 1000.0f) * transform;
    transform = MatrixUtils::rotateByY<float>(135.0) * transform;
    transform = MatrixUtils::translate(-200.0f, -50.0f, 0.0f) * transform;

    std::vector<Vector3f> vertices;
    vertices.resize(vertexCount);
    for (int i = 0; i < vertexCount; ++i) {
        float x, y, z;
//        fscanf(fp, "%f %f %f\n", &vertices[i][0], &vertices[i][1], &vertices[i][2]);
        fscanf(fp, "%f %f %f\n", &x, &y, &z);
        vertices[i] = (transform * Vector4f{x, y, z, 1.0f}).getXYZ();
    }

    std::vector<Vector3i> indices;
    indices.resize(faceCount * 3);
    for (int i = 0; i < faceCount; ++i) {
        int n = 0;
        fscanf(fp, "%d", &n);
        assert(n == 3);
        fscanf(fp, " %d %d %d\n", &indices[i][0], &indices[i][1], &indices[i][2]);
    }
//    assert(ftell(fp) == 0);
    fclose(fp);

    return std::make_unique<Mesh>(vertices, indices);
}

int main() {
    auto moonTexture = loadMoonTexture();

    const float scale = 4.0f;
    const Vector2f viewportSize = {640 * scale, 480 * scale};
    const SizeI outputPixelSize = {static_cast<int>(viewportSize.getX()), static_cast<int>(viewportSize.getY())};

    const SizeF cameraFrameSize = {static_cast<float>(outputPixelSize.getWidth()) * 1.0f,
                                   static_cast<float>(outputPixelSize.getHeight()) * 1.0f};
    const float cameraNear = -0.1f;
    const float cameraFar = -100.0f;
    const auto fov = static_cast<float>(60.0f * M_PI / 180.0f);

    const Vector3f cameraOrigin{150.0f, 220.0f, 500.0f};
    const Vector3f cameraTarget{0.0f, 100.0f, 50.0f};

    const auto viewPortTransform = MatrixUtils::makeViewportTransform(viewportSize.getX(), viewportSize.getY());

    const auto aspect = viewportSize.getX() / viewportSize.getY();
    float left, right, bottom, top;
    MatrixUtils::getViewPlaneBounds(fov, aspect, cameraNear, cameraFar, left, right, bottom, top);

    const auto world2cameraTransform = MatrixUtils::makeWorldToCameraTransform(cameraOrigin,
                                                                               cameraTarget,
                                                                               Vector3f{0.0f, 1.0f, 0.0f});
    const auto camera2worldTransform = MatrixUtils::makeCameraToWorldTransform(cameraOrigin,
                                                                               cameraTarget,
                                                                               Vector3f{0.0f, 1.0f, 0.0f});

    std::vector<LightSource> lightSources = {
            {{150.0f * 4.0f,  500.0f * 1.5f, 1200.0f}, {1.0f, 1.0f, 1.0f}},
            {{-150.0f * 1.0f, 400.0f,        400.0f},  {0.3f, 0.3f, 0.3f}},
//            {{0.0f, 120.0f,        -80.0f},  {0.3f, 0.3f, 0.3f}},
    };

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

//    {
//        const auto material = Material({0.1f, 0.1f, 0.1f},
//                                       {0.3f, 0.3f, 0.3f},
//                                       {0.0f, 0.0f, 0.0f},
//                                       0.0f);
//        auto sphere = std::make_shared<Sphere>(Vector3f{80.0f, 30.0f, 50.0f}, 30.0f);
//        sphere->setTexture(moonTexture);
//        scene.addSurface(sphere);
//    }

    {
        const auto material = Material({0.01f, 0.01f, 0.01f},
                                       {0.1f, 0.5f, 0.1f},
                                       {0.0f, 0.0f, 0.0f},
                                       0.0f);
        scene.addSurface(std::make_shared<Sphere>(Vector3f{130.0f, 50.0f, -50.0f}, 50.0f, material));
    }


    {
        const Material& triangleMaterial = Material({0.01f, 0.01f, 0.01f},
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
                                       {0.3f, 0.3f, 0.3f},
                                       {0.0f, 0.0f, 0.0f},
                                       0.0f);
        auto moonSphere = std::make_shared<Sphere>(Vector3f{350.0f, 200.0f, -500.0f}, 200.0f,
                                                   material);
        moonSphere->setTexture(moonTexture);
        scene.addSurface(moonSphere);
    }

    {
        const Material& material = Material({},
                                            {0.2f, 0.2f, 0.2f},
                                            {0.03f, 0.03f, 0.03f},
                                            0.0f);
        scene.addSurface(std::make_shared<Plane>(Vector3f{0.0f, 1.0f, 0.0f},
                                                 Vector3f{0.0f, 0.0f, 0.0f}, material));
    }

    {
        std::shared_ptr<Mesh> dragon = loadDragonMesh();
        const auto material = Material({0.01f, 0.01f, 0.01f},
                                       {0.8f, 0.2f, 0.2f},
                                       {.3f, .3f, .3f},
                                       200.0f);
        dragon->setMaterial(material);
        scene.addSurface(dragon);
    }

    auto outputRGBBuffer = std::make_unique<uint8_t[]>(
            outputPixelSize.getWidth() * outputPixelSize.getHeight() * 3);

    #pragma omp parallel for
    for (int j = 0; j < outputPixelSize.getHeight(); ++j) {
        #pragma omp parallel for
        for (int i = 0; i < outputPixelSize.getWidth(); ++i) {
            const auto dx = left + (right - left) * (static_cast<float>(i) + 0.5f) /
                                   static_cast<float>(outputPixelSize.getWidth());
            const auto dy = top - (top - bottom) * (static_cast<float>(j) + 0.5f) /
                                  static_cast<float>(outputPixelSize.getHeight());
            const Vector4f pointInCamera = {
                    dx,
                    dy,
                    cameraNear,
                    1.0f
            };

            const Vector4f pointInWorld4Homo = camera2worldTransform * pointInCamera;
            const Vector3f pointInWorld = {pointInWorld4Homo.getX(), pointInWorld4Homo.getY(),
                                           pointInWorld4Homo.getZ()};

            const auto rayDirection = (pointInWorld - cameraOrigin).normalize();
            const auto rayOrigin = cameraOrigin;
            const auto ray = Ray{rayOrigin, rayDirection};

            const auto color = rayColor(scene, lightSources, ray, 0.0f, std::numeric_limits<float>::max());

            const int index = (j * outputPixelSize.getWidth() + i) * 3;
            outputRGBBuffer[index + 0] = static_cast<uint8_t>(std::min(1.0f, color.getX()) * 255);
            outputRGBBuffer[index + 1] = static_cast<uint8_t>(std::min(1.0f, color.getY()) * 255);
            outputRGBBuffer[index + 2] = static_cast<uint8_t>(std::min(1.0f, color.getZ()) * 255);

            int currentCount = j * outputPixelSize.getWidth() + i;
            int totalCount = outputPixelSize.getWidth() * outputPixelSize.getHeight();
            std::cout << "Calculating pixel count " << currentCount
                      << "/" << totalCount << " progress:" << (currentCount * 100.0f / totalCount) << "%" << std::endl;
        }
    }

    FILE *fp = fopen("../out/test.png", "wb");
    assert(fp);
    svpng(fp, outputPixelSize.getWidth(), outputPixelSize.getHeight(), outputRGBBuffer.get(), 0);
    fclose(fp);

    return 0;
}
