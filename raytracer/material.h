
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

#endif // MATERIAL_H
