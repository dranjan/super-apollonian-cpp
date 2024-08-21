#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <cmath>

#include "riemann_sphere.hpp"
#include "mobius.hpp"

namespace apollonian {

/* A generalized circle is defined as the locus of a sesquilinear form
 *
 *     adjoint(z)*C*z == 0,
 *
 * where z is a projective complex number and C is a self-adjoint 2 by 2
 * complex matrix with determinant -1.
 *
 * -C and C define the same equality locus, so the matrix actually
 * contains one extra bit of information, which can be used to select
 * one component of the complement of the circle according to
 *
 *     adjoint(z)*C*z <= 0.
 *
 * Alernatively, the extra bit can be thought of as selecting an
 * orientation of the circle according to some convention.
 *
 * The "disk" interpretation in particular is pretty useful for us, so
 * we choose signs consistently.
 */
class circle {
public:
    circle() = default;
    circle(double v00, const dcomplex& v01, double v11);
    circle(const dcomplex& center, double radius);
    circle(const pcomplex& z0, const pcomplex& z1, const pcomplex& z2);

    dcomplex center() const;
    double radius() const;

    circle reverse() const;

    double operator () (const pcomplex& z) const;

public:
    /* The full matrix of the quadratic form is
     *         v00_   v01_
     *    conj(v01_)  v11_.
     */
    double  v00_;
    dcomplex v01_;
    double  v11_;
};

template <>
inline circle
mobius_transformation::operator () (const circle& t) const {
    const dcomplex& v00 =  v11_;
    const dcomplex& v01 = -v01_;
    const dcomplex& v10 = -v10_;
    const dcomplex& v11 =  v00_;

    dcomplex v00h = std::conj(v00);
    dcomplex v01h = std::conj(v10);
    dcomplex v10h = std::conj(v01);
    dcomplex v11h = std::conj(v11);

    const dcomplex& c00 = t.v00_;
    const dcomplex& c01 = t.v01_;
    dcomplex        c10 = std::conj(t.v01_);
    const dcomplex& c11 = t.v11_;

    dcomplex w00 = c00*v00 + c01*v10;
    dcomplex w01 = c00*v01 + c01*v11;
    dcomplex w10 = c10*v00 + c11*v10;
    dcomplex w11 = c10*v01 + c11*v11;

    return {(v00h*w00 + v01h*w10).real(),
             v00h*w01 + v01h*w11,
            (v10h*w01 + v11h*w11).real()};
}

inline
circle::circle(double v00, const dcomplex& v01, double v11)
    : v00_{v00}, v01_{v01}, v11_{v11}
{
}

inline
circle::circle(const dcomplex& center, double radius)
    : v00_{1/radius}, v01_{-center/radius},
      v11_{std::norm(center)/radius - radius}
{
}

inline
circle::circle(
        const pcomplex& z0, const pcomplex& z1, const pcomplex& z2)
{
    mobius_transformation m
        = mobius_transformation::cross_ratio(z0, z1, z2).inverse();
    *this = m(circle{0, -1.0, 0});
}

inline dcomplex
circle::center() const {
    return -v01_/v00_;
}

inline double
circle::radius() const {
    return std::sqrt(std::norm(v01_) - v00_*v11_)/v00_;
}

inline circle
circle::reverse() const {
    return {-v00_, -v01_, -v11_};
}

inline double
circle::operator () (const pcomplex& z) const {
    return v00_*std::norm(z.v0_) +
           2*(v01_*std::conj(z.v0_)*z.v1_).real() +
           v11_*std::norm(z.v1_);
}

} // appollonian

#endif // CIRCLE_HPP
