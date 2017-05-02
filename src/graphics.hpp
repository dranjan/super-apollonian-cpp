#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "color.hpp"
#include "image_buffer.hpp"

template <>
inline uint8_t RGBATraits<apollonian::RGBColor>::r() const {
    return uint8_t(clamp(pixel.r_, 0.0, 1.0)*0xff);
}

template <>
inline uint8_t RGBATraits<apollonian::RGBColor>::g() const {
    return uint8_t(clamp(pixel.g_, 0.0, 1.0)*0xff);
}

template <>
inline uint8_t RGBATraits<apollonian::RGBColor>::b() const {
    return uint8_t(clamp(pixel.b_, 0.0, 1.0)*0xff);
}

template <>
inline uint8_t RGBATraits<apollonian::RGBColor>::a() const {
    return 0xff;
}

void
draw_circle(ImageBuffer<apollonian::RGBColor>& image,
            double xc, double yc, double r,
            const apollonian::RGBColor& new_color,
            const apollonian::RGBColor& old_color);

#endif // GRAPHICS_HPP
