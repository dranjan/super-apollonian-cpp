#ifndef COLOR_HPP
#define COLOR_HPP

#include <stdint.h>

namespace apollonian {

class RGBColor {
public:
    RGBColor() = default;
    RGBColor(double r, double g, double b);
    RGBColor(uint32_t h);

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

inline RGBColor::RGBColor(uint32_t h) {
    b_ = (h & 0xff) / 255.0;
    h >>= 8;
    g_ = (h & 0xff) / 255.0;
    h >>= 8;
    r_ = (h & 0xff) / 255.0;
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
