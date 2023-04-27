
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
        metal(const colour& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        colour albedo;
        double fuzz;
};

class dielectric : public material {
    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered
        ) const override {
            attenuation = colour(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

            vec3 unit_direction = unit_vector(r_in.direction());
            vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);

            scattered = ray(rec.p, refracted);
            return true;
        }

    public:
        double ir; // index of refraction
};

#endif // MATERIAL_H
