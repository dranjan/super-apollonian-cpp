#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "color.hpp"
#include "image_buffer.hpp"

void
draw_circle(ImageBuffer<apollonian::RGBColor>& image,
            double xc, double yc, double r,
            const apollonian::RGBColor& new_color,
            const apollonian::RGBColor& old_color);

void
draw_circle_complement(ImageBuffer<apollonian::RGBColor>& image,
                       double xc, double yc, double r,
                       const apollonian::RGBColor& new_color,
                       const apollonian::RGBColor& old_color);

void
draw_half_plane(ImageBuffer<apollonian::RGBColor>& image,
                double a, double b, double c,
                const apollonian::RGBColor& new_color,
                const apollonian::RGBColor& old_color);

#endif // GRAPHICS_HPP
