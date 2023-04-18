cmake_minimum_required(VERSION 3.21)
project(raytracer)

set(CMAKE_CXX_STANDARD 14)

add_executable(raytracer main.cpp vec3.h colour.h ray.h)
