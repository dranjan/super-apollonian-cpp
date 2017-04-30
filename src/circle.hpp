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
class Circle {
public:
    Circle() = default;
    Circle(double v00, const Complex& v01, double v11);
    Circle(const Complex& center, double radius);
    Circle(const PComplex& z0, const PComplex& z1, const PComplex& z2);

    Complex center() const;
    double radius() const;

    Circle reverse() const;

    double operator () (const PComplex& z) const;

public:
    /* The full matrix of the quadratic form is
     *         v00_   v01_
     *    conj(v01_)  v11_.
     */
    double  v00_;
    Complex v01_;
    double  v11_;
};

template <>
inline Circle
MobiusTransformation::operator () (const Circle& t) const {
    const Complex& v00 =  v11_;
    const Complex& v01 = -v01_;
    const Complex& v10 = -v10_;
    const Complex& v11 =  v00_;

    Complex v00h = std::conj(v00);
    Complex v01h = std::conj(v10);
    Complex v10h = std::conj(v01);
    Complex v11h = std::conj(v11);

    const Complex& c00 = t.v00_;
    const Complex& c01 = t.v01_;
    Complex        c10 = std::conj(t.v01_);
    const Complex& c11 = t.v11_;

    Complex w00 = c00*v00 + c01*v10;
    Complex w01 = c00*v01 + c01*v11;
    Complex w10 = c10*v00 + c11*v10;
    Complex w11 = c10*v01 + c11*v11;

    return {(v00h*w00 + v01h*w10).real(),
             v00h*w01 + v01h*w11,
            (v10h*w01 + v11h*w11).real()};
}

inline
Circle::Circle(double v00, const Complex& v01, double v11)
    : v00_{v00}, v01_{v01}, v11_{v11}
{
}

inline
Circle::Circle(const Complex& center, double radius)
    : v00_{1/radius}, v01_{center/radius},
      v11_{std::norm(center)/radius - radius}
{
}

inline
Circle::Circle(
        const PComplex& z0, const PComplex& z1, const PComplex& z2)
{
    MobiusTransformation m
        = MobiusTransformation::cross_ratio(z0, z1, z2).inverse();
    *this = m(Circle{0, -1.0, 0});
}

inline Complex
Circle::center() const {
    return -v01_/v00_;
}

inline double
Circle::radius() const {
    return std::sqrt(std::norm(v01_) - v00_*v11_)/v00_;
}

inline Circle
Circle::reverse() const {
    return {-v00_, -v01_, -v11_};
}

inline double
Circle::operator () (const PComplex& z) const {
    return v00_*std::norm(z.v0_) +
           2*(v01_*z.v0_*std::conj(z.v1_)).real() +
           v11_*std::norm(z.v1_);
}

} // appollonian

#endif // CIRCLE_HPP
