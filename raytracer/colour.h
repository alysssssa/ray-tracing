
#ifndef COLOUR_H
#define COLOUR_H

#include "vec3.h"
#include <iostream>

void write_colour(std::ostream &out, colour pixel_colour, int samples_per_pixel) {
    auto r = pixel_colour.x();
    auto g = pixel_colour.y();
    auto b = pixel_colour.z();

    // divide the colour by the number of samples
    // apply gamma correction of 2.0 (raise colour to the power 1/gamma)
    // gamma correction is used to "undo" the gamma correction applied to the image by the display
    // because almost all image viewers assume the image is gamma corrected before being stored as a byte
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // write the translated [0, 255] value of each colour component
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << " "
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << " "
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << "\n";
}

#endif // COLOUR_H
