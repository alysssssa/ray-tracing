
#ifndef CAMERA_H
#define CAMERA_H

#include "raytracer.h"

class camera {
    public:
        camera(
                point3 lookfrom, // camera origin
                point3 lookat, // camera target
                vec3 vup, // view up
                double vfov, // vertical field-of-view in degrees
                double aspect_ratio
            ) {
            // x - right, y - up, z - out of the screen
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            // w - camera's forward direction
            auto w = unit_vector(lookfrom - lookat);
            // u - camera's right direction
            auto u = unit_vector(cross(vup, w));
            // v - camera's up direction
            auto v = cross(w, u);

            origin = lookfrom;
            horizontal = viewport_width * u;
            vertical = viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - w;
        }

        ray get_ray(double s, double t) const {
            return ray(origin, lower_left_corner + s*horizontal + t*vertical - origin);
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};


#endif // CAMERA_H
