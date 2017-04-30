#ifndef BOX_HPP
#define BOX_HPP

#include "riemann_sphere.hpp"
#include "circle.hpp"
#include "apollonian.hpp"

namespace apollonian {

enum class IntersectionType {
    Invalid = 0,
    Inside = 1,
    Outside = 2,
    Intersects = 3,
};

/* Axis-aligned box. */
class Box {
public:
    double xmin;
    double xmax;
    double ymin;
    double ymax;

    IntersectionType intersects_circle(const Circle& circle) const;
};

inline Box
make_box(const Complex& center, double width, double height) {
    return {center.real() - width/2,
            center.real() + width/2,
            center.imag() - height/2,
            center.imag() + height/2};
}

template <typename T>
inline T square(T t) {
    return t*t;
}

inline IntersectionType
Box::intersects_circle(const Circle& circle) const {
    if (circle.v00_ <= 0) {
        /* Half space or disk complement */
        if (circle(Complex(xmin, ymin)) >= 0 &&
            circle(Complex(xmin, ymax)) >= 0 &&
            circle(Complex(xmax, ymin)) >= 0 &&
            circle(Complex(xmax, ymax)) >= 0)
        {
            return IntersectionType::Outside;
        }

        return IntersectionType::Intersects;
    } else {
        /* Disk */
        Complex center = circle.center();
        double r = circle.radius();
        double x = center.real();
        double y = center.imag();
        if (xmin + r <= x && x <= xmax - r &&
            ymin + r <= y && y <= ymax - r)
        {
            return IntersectionType::Inside;
        }
        double s = 0;
        if      (x < xmin) s += square(x - xmin);
        else if (x > xmax) s += square(x - xmax);
        if      (y < ymin) s += square(y - ymin);
        else if (y > ymax) s += square(y - ymax);

        if (s <= square(r)) return IntersectionType::Intersects;

        return IntersectionType::Outside;
    }
}

} // apollonian

#endif // BOX_HPP
