cmake_minimum_required(VERSION 3.22)
project(CpuRayTracing)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-O3)
add_executable(CpuRayTracing main.cpp
        third_party/svpng/svpng.inc
        src/Vector.h
        src/Matrix.h
        src/Matrix.cpp
        src/Common.h
        src/Size.h
        src/Rect.h
        src/Ray.h
        src/Rasterer.cpp
        src/Rasterer.h
        src/Sphere.h
        src/Sphere.cpp
        src/Triangle.h
        src/Triangle.cpp
        src/Surface.h
        src/HitRecord.h
        src/Plane.cpp
        src/Plane.h
        src/Material.h
        src/Scene.cpp
        src/Scene.h
        src/Texture2D.h
        src/Texture2D.cpp
        src/MatrixUtils.h
        src/Mesh.cpp
        src/Mesh.h
        src/MathUtils.cpp
        src/MathUtils.h
        src/BoundingBox.cpp
        src/BoundingBox.h)

add_subdirectory(test)