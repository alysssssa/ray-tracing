
#include "raytracer.h"
#include "colour.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include <iostream>

// ray tracer:
// 1. calculate the ray from the eye to the pixel
// 2. determine which objects the ray intersects
// 3. compute a colour for that intersection point

colour ray_colour(const ray& r, const hittable& world) {
    hit_record rec;

    // visualize the normal: map each component to the interval [0,1]
    // then map x/y/z to r/g/b
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5*(rec.normal + colour(1, 1, 1));
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

    // world
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

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
                pixel_colour += ray_colour(r, world);
            }
            write_colour(std::cout, pixel_colour, samples_per_pixel);
        }
    }
        std::cerr << "\nDone.\n";
}
