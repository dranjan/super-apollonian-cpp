#ifndef COLOR_HPP
#define COLOR_HPP

#include <stdint.h>

namespace apollonian {

class rgb_color {
public:
    rgb_color() = default;
    rgb_color(double r, double g, double b);

    static rgb_color
    decode24(uint32_t h);

    rgb_color& operator *= (double alpha);
    rgb_color& operator /= (double alpha);
    rgb_color& operator += (const rgb_color& other);
    rgb_color& operator -= (const rgb_color& other);

    rgb_color operator * (double alpha) const;
    rgb_color operator / (double alpha) const;
    rgb_color operator + (const rgb_color& other) const;
    rgb_color operator - (const rgb_color& other) const;
    rgb_color blend(const rgb_color& other, double alpha) const;

    static const rgb_color white;
    static const rgb_color black;

public:
    int32_t r_;
    int32_t g_;
    int32_t b_;
};

inline rgb_color::rgb_color(double r, double g, double b)
    : r_{int32_t(r*0x7fffffff)},
      g_{int32_t(g*0x7fffffff)},
      b_{int32_t(b*0x7fffffff)}
{
}

inline rgb_color
rgb_color::decode24(uint32_t h) {
    double b = (h & 0xff) / 255.0;
    h >>= 8;
    double g = (h & 0xff) / 255.0;
    h >>= 8;
    double r = (h & 0xff) / 255.0;
    return rgb_color(r, g, b);
}

inline rgb_color&
rgb_color::operator *= (double alpha) {
    r_ *= alpha;
    g_ *= alpha;
    b_ *= alpha;

    return *this;
}

inline rgb_color&
rgb_color::operator /= (double alpha) {
    r_ /= alpha;
    g_ /= alpha;
    b_ /= alpha;

    return *this;
}

inline rgb_color&
rgb_color::operator += (const rgb_color& other) {
    r_ += other.r_;
    g_ += other.g_;
    b_ += other.b_;

    return *this;
}

inline rgb_color&
rgb_color::operator -= (const rgb_color& other) {
    r_ -= other.r_;
    g_ -= other.g_;
    b_ -= other.b_;

    return *this;
}

inline rgb_color
rgb_color::operator * (double alpha) const {
    rgb_color value(*this);
    value *= alpha;
    return value;
}

inline rgb_color
rgb_color::operator / (double alpha) const {
    rgb_color value(*this);
    value /= alpha;
    return value;
}

inline rgb_color
rgb_color::operator + (const rgb_color& other) const {
    rgb_color value(*this);
    value += other;
    return value;
}

inline rgb_color
rgb_color::operator - (const rgb_color& other) const {
    rgb_color value(*this);
    value -= other;
    return value;
}

inline rgb_color
rgb_color::blend(const rgb_color& other, double alpha) const {
    double beta = 1.0 - alpha;
    return rgb_color(r_*beta + other.r_*alpha,
                     g_*beta + other.g_*alpha,
                     b_*beta + other.b_*alpha);
}

} // apollonian

#endif // COLOR_HPP
