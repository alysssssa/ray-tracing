
#include "raytracer.h"
#include "colour.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "moving_sphere.h"
#include "bvh.h"

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

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<checker_texture>(colour(0.2, 0.3, 0.1), colour(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(ground_material)));

    for (int a = -11; a < 11; a++)
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            // don't let spheres intersect with each other
            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                // diffuse
                if (choose_mat < 0.8) {
                    auto albedo = colour::random() * colour::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                    // motion blur
                    // auto center2 = center + vec3(0, random_double(0, 0.5), 0);
                    // world.add(make_shared<moving_sphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
                // metal
                } else if (choose_mat < 0.95) {
                    auto albedo = colour::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                // glass
                } else {
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }

    // three big spheres
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0,1,0), 1.0, material1));

    auto material2 = make_shared<lambertian>(colour(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4,1,0), 1.0, material2));

    auto material3 = make_shared<metal>(colour(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4,1,0), 1.0, material3));

    return {make_shared<bvh_node>(world, 0.0, 1.0)};
}

hittable_list two_spheres() {
    hittable_list objects;

    auto checker = make_shared<checker_texture>(colour(0.2, 0.3, 0.1), colour(0.9, 0.9, 0.9));

    objects.add(make_shared<sphere>(point3(0,-10,0), 10, make_shared<lambertian>(checker)));
    objects.add(make_shared<sphere>(point3(0,10,0), 10, make_shared<lambertian>(checker)));

    return objects;
}

hittable_list two_perlin_spheres() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(2);
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

    return objects;
}

int main() {

    // write output image in ppm
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 50;
    const int max_depth = 50;

    // world
    hittable_list world;

    point3 lookfrom;
    point3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;

    switch (0) {
        case 1:
            world = random_scene();
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            aperture = 0.1;
            break;

        case 2:
            world = two_spheres();
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            break;

        default:
        case 3:
            world = two_perlin_spheres();
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            break;
    }

    // camera with depth of field
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

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
