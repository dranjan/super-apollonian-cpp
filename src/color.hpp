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

    RGBColor& operator *= (double alpha);
    RGBColor& operator /= (double alpha);
    RGBColor& operator += (const RGBColor& other);
    RGBColor& operator -= (const RGBColor& other);

    RGBColor operator * (double alpha) const;
    RGBColor operator / (double alpha) const;
    RGBColor operator + (const RGBColor& other) const;
    RGBColor operator - (const RGBColor& other) const;
    RGBColor blend(const RGBColor& other, double alpha) const;

    static const RGBColor white;
    static const RGBColor black;

public:
    int32_t r_;
    int32_t g_;
    int32_t b_;
};

inline RGBColor::RGBColor(double r, double g, double b)
    : r_{int32_t(r*0x7fffffff)},
      g_{int32_t(g*0x7fffffff)},
      b_{int32_t(b*0x7fffffff)}
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

inline RGBColor&
RGBColor::operator *= (double alpha) {
    r_ *= alpha;
    g_ *= alpha;
    b_ *= alpha;

    return *this;
}

inline RGBColor&
RGBColor::operator /= (double alpha) {
    r_ /= alpha;
    g_ /= alpha;
    b_ /= alpha;

    return *this;
}

inline RGBColor&
RGBColor::operator += (const RGBColor& other) {
    r_ += other.r_;
    g_ += other.g_;
    b_ += other.b_;

    return *this;
}

inline RGBColor&
RGBColor::operator -= (const RGBColor& other) {
    r_ -= other.r_;
    g_ -= other.g_;
    b_ -= other.b_;

    return *this;
}

inline RGBColor
RGBColor::operator * (double alpha) const {
    RGBColor value(*this);
    value *= alpha;
    return value;
}

inline RGBColor
RGBColor::operator / (double alpha) const {
    RGBColor value(*this);
    value /= alpha;
    return value;
}

inline RGBColor
RGBColor::operator + (const RGBColor& other) const {
    RGBColor value(*this);
    value += other;
    return value;
}

inline RGBColor
RGBColor::operator - (const RGBColor& other) const {
    RGBColor value(*this);
    value -= other;
    return value;
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
