
#ifndef AABB_H
#define AABB_H

#include "raytracer.h"

class aabb {

    public:
        aabb() {}
        aabb(const point3& a, const point3& b) { minimum = a; maximum = b; }

        point3 min() const { return minimum; }
        point3 max() const { return maximum; }

        bool hit(const ray& r, double tmin, double tmax) const {
            for (int a = 0; a < 3; a++) {
                auto invD = 1.0f / r.direction()[a];
                auto t0 = (min()[a] - r.origin()[a]) * invD;
                auto t1 = (max()[a] - r.origin()[a]) * invD;
                if (invD < 0.0f)
                    std::swap(t0, t1);
                tmin = t0 > tmin ? t0 : tmin;
                tmax = t1 < tmax ? t1 : tmax;
                if (tmax <= tmin)
                    return false;
            }
            return true;
        }

        point3 minimum;
        point3 maximum;
};


#endif // AABB_H
