#ifndef BOX_HPP
#define BOX_HPP

#include "riemann_sphere.hpp"
#include "circle.hpp"
#include "apollonian.hpp"

namespace apollonian {

enum class intersection_type {
    invalid = 0,
    inside = 1,
    outside = 2,
    intersects = 3,
};

/* Axis-aligned box. */
class box {
public:
    intersection_type intersects_circle(const circle& c) const;

public:
    double xmin;
    double xmax;
    double ymin;
    double ymax;
};

inline box
make_box(const dcomplex& center, double width, double height) {
    return {center.real() - width/2,
            center.real() + width/2,
            center.imag() - height/2,
            center.imag() + height/2};
}

template <typename T>
inline T square(T t) {
    return t*t;
}

inline intersection_type
box::intersects_circle(const circle& c) const {
    if (c.v00_ <= 0) {
        /* Half space or disk complement */
        if (c(dcomplex(xmin, ymin)) >= 0 &&
            c(dcomplex(xmin, ymax)) >= 0 &&
            c(dcomplex(xmax, ymin)) >= 0 &&
            c(dcomplex(xmax, ymax)) >= 0)
        {
            return intersection_type::outside;
        }

        return intersection_type::intersects;
    } else {
        /* Disk */
        dcomplex center = c.center();
        double r = c.radius();
        double x = center.real();
        double y = center.imag();
        if (xmin + r <= x && x <= xmax - r &&
            ymin + r <= y && y <= ymax - r)
        {
            return intersection_type::inside;
        }
        double s = 0;
        if      (x < xmin) s += square(x - xmin);
        else if (x > xmax) s += square(x - xmax);
        if      (y < ymin) s += square(y - ymin);
        else if (y > ymax) s += square(y - ymax);

        if (s <= square(r)) return intersection_type::intersects;

        return intersection_type::outside;
    }
}

} // apollonian

#endif // BOX_HPP
