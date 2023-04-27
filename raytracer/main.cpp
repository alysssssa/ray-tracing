
#include "raytracer.h"
#include "colour.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>

// ray tracer:
// 1. calculate the ray from the eye to the pixel
// 2. determine which objects the ray intersects
// 3. compute a colour for that intersection point

colour ray_colour(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // limit the maximum recursion depth, returning no light contribution at the maximum depth
    if (depth <= 0)
        return {0,0,0};

    // generating a random diffuse bounce ray from hit point to random point (on the unit sphere from center p+n)
    // shadow acne: ignore hits very near zero (t_min = 0.001)
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        colour attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_colour(scattered, world, depth-1);
        return {0,0,0};
    }

    // return colour of the background
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    // linear interpolation between white (t=0) and blue (t=1)
    // blended_value = (1-t)*start_value + t*end_value
    return (1.0-t)*colour(1.0, 1.0, 1.0) + t*colour(0.5, 0.7, 1.0);
}

int main() {

    // write output image in ppm
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // world
    hittable_list world;

    auto material_ground = make_shared<lambertian>(colour(0.8, 0.8, 0.0));
    auto material_center = make_shared<dielectric>(1.5);
    auto material_left = make_shared<dielectric>(1.5);
    auto material_right = make_shared<metal>(colour(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3(1.0,    0.0, -1.0), 0.5, material_right));

    // camera
    camera cam;

    // ppm format:
    // P3 - colours in ASCII; column number; row number; 255 - for max colour;
    // RGB triplets
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        // write progress indicator to the error output stream
        std::cerr << "\rScanline remaining: " << j << " " << std::flush;
        for (int i = 0; i < image_width; ++i) {
            colour pixel_colour(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_colour += ray_colour(r, world, max_depth);
            }
            write_colour(std::cout, pixel_colour, samples_per_pixel);
        }
    }
        std::cerr << "\nDone.\n";
}
