
#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
#include <random>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// alternative random number generator
//inline double random_double() {
//    // random number in [0,1)
//    return rand() / (RAND_MAX + 1.0);
//}

inline double random_double() {
    // random number in [0,1)
    static std::uniform_real_distribution<double> distribution(0.0,1.0);
    static std::random_device dev;
    static std::mt19937 generator(dev());
    return distribution(generator);
}

inline double random_double(double min, double max) {
    // random number in [min,max)
    return min + (max-min)*random_double();
}

inline int random_int(int min, int max) {
    // random integer in [min,max]
    return static_cast<int>(random_double(min, max+1));
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Common Headers
#include "ray.h"
#include "vec3.h"

#endif // RAYTRACER_H
