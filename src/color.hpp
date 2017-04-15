#ifndef COLOR_HPP
#define COLOR_HPP

#include <stdint.h>

namespace apollonian {

class RGBColor {
public:
    RGBColor() = default;
    RGBColor(double r, double g, double b);
    RGBColor(uint32_t h);

    RGBColor blend(const RGBColor& other, double alpha);

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

} // apollonian

#endif // COLOR_HPP
