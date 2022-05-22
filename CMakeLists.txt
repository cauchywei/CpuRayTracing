cmake_minimum_required(VERSION 3.22)
project(CpuRayTracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(CpuRayTracing main.cpp src/Vector.h src/Matrix.h third_party/svpng/svpng.inc src/Common.h src/Size.h src/Rect.h src/Ray.h src/Rasterer.cpp src/Rasterer.h src/Sphere.h src/Sphere.cpp src/Matrix.cpp src/Triangle.h src/Triangle.cpp)

add_subdirectory(test)