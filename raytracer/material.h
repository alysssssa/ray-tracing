
#ifndef MATERIAL_H
#define MATERIAL_H

#include "raytracer.h"

// abstract class for materials
// 1. produce a scattered ray (or say that the incident ray is absorbed)
// 2. compute the attenuation of the scattered ray, if scattered

struct hit_record;

class material {
    public:
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered
        ) const = 0;
};

class lambertian : public material {
    public:
        lambertian(const colour& a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered
        ) const override {
            auto scatter_direction = rec.normal + random_unit_vector(); // lambertian diffuse
            //auto scatter_direction = random_in_hemisphere(rec.normal); // hemispherical scattering

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    public:
        colour albedo;
};

class metal : public material {
    public:
        metal(const colour& a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        colour albedo;
        double fuzz;
};

#endif // MATERIAL_H
