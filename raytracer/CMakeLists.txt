cmake_minimum_required(VERSION 3.21)
project(raytracer)

set(CMAKE_CXX_STANDARD 14)

add_executable(raytracer main.cpp vec3.h colour.h ray.h hittable.h sphere.h hittable_list.h raytracer.h camera.h material.h moving_sphere.h aabb.h bvh.h texture.h perlin.h aarect.h box.h constant_medium.h)
