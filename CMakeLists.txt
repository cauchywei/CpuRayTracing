cmake_minimum_required(VERSION 3.22)
project(CpuRayTracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(CpuRayTracing main.cpp math/vector.h math/matrix.h third_party/svpng/svpng.inc math/detail/vector2.h)

add_subdirectory(test)