
#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "raytracer.h"
#include "aabb.h"

class material;

// when a ray hits a surface (e.g. a sphere), the material pointer in the hit_record will be set to point
// at the material pointer the sphere was created with
// when the ray_colour() routine gets the hit_record, it can call member functions of the material pointer
// to find out what ray, if any, is scattered
struct hit_record {
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    double u;
    double v;
    bool front_face; // determine which side of the surface the ray is hitting

    // normal always points against the ray
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
        // not all primitives have bounding boxes (e.g. infinite plane)
        // moving objects have bounding box enclosing the object for the entire time interval
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
};

#endif // HITTABLE_H
