#ifndef BOX_HPP
#define BOX_HPP

#include "riemann_sphere.hpp"

namespace apollonian {

/* Axis-aligned box. */
class Box {
public:
    double xmin;
    double xmax;
    double ymin;
    double ymax;
};

inline Box
make_box(const Complex& center, double width, double height) {
    return {center.real() - width/2,
            center.real() + width/2,
            center.imag() - height/2,
            center.imag() + height/2};
}

} // apollonian

#endif // BOX_HPP
