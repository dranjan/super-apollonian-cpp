#ifndef COLOR_HPP
#define COLOR_HPP

#include <stdint.h>

namespace apollonian {

class RGBColor {
public:
    RGBColor() = default;
    RGBColor(double r, double g, double b);

    static RGBColor
    decode24(uint32_t h);

    RGBColor blend(const RGBColor& other, double alpha) const;

    static const RGBColor white;
    static const RGBColor black;

public:
    double r_;
    double g_;
    double b_;
};

inline RGBColor::RGBColor(double r, double g, double b)
    : r_{r}, g_{g}, b_{b}
{
}

inline RGBColor
RGBColor::decode24(uint32_t h) {
    double b = (h & 0xff) / 255.0;
    h >>= 8;
    double g = (h & 0xff) / 255.0;
    h >>= 8;
    double r = (h & 0xff) / 255.0;
    return RGBColor(r, g, b);
}

inline RGBColor
RGBColor::blend(const RGBColor& other, double alpha) const {
    double beta = 1.0 - alpha;
    return RGBColor(r_*beta + other.r_*alpha,
                    g_*beta + other.g_*alpha,
                    b_*beta + other.b_*alpha);
}

} // apollonian

#endif // COLOR_HPP
