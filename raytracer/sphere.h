
#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        sphere() {}
        sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 center;
        double radius;
        shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // ray-sphere intersection
    // ray: p(t) = A + t*B
    // sphere: (p-C).(p-C) = r^2
    // (A + t*B - C).(A + t*B - C) = r^2
    // t^2*B.B + 2*t*B.(A-C) + (A-C).(A-C) - r^2 = 0
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // find the nearest root that lies in the acceptable range
    auto root = (-half_b - sqrtd) / a;
    // if root is not in the acceptable range, try the other root
    if (root < t_min || t_max < root) {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || t_max < root) {
                return false;
            }
        }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;

}

#endif // SPHERE_H
