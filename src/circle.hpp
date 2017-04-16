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
    Circle(const Complex& v00, const Complex& v01,
           const Complex& v10, const Complex& v11);
    Circle(const Complex& center, double radius);
    Circle(const PComplex& z0, const PComplex& z1, const PComplex& z2);
    Circle(const Matrix& v);

    Complex center() const;
    double radius() const;

public:
    /* XXX This representation is wasteful, since a generalized circle
     * only has three degrees of freedom.
     */
    Matrix v_;
};

template <>
inline Circle
MobiusTransformation::operator () (const Circle& t) const {
    MobiusTransformation m = inverse();
    return Circle(m.v_.adjoint()*t.v_*m.v_);
}

inline
Circle::Circle(const Complex& v00, const Complex& v01,
               const Complex& v10, const Complex& v11)
{
    v_ << v00, v01,
          v10, v11;
}

inline
Circle::Circle(const Complex& center, double radius)
{
    v_ << 1,                  center,
          std::conj(center),  std::norm(center) - radius*radius;
    v_ /= radius;
}

inline
Circle::Circle(const Matrix& v)
    : v_{v}
{
}

inline
Circle::Circle(
        const PComplex& z0, const PComplex& z1, const PComplex& z2)
{
    MobiusTransformation m
        = MobiusTransformation::cross_ratio(z0, z1, z2).inverse();
    Complex f = Complex(1.0)/det(m.v_);
    *this = m(Circle{0, -f,
                     f,  0});
}

inline Complex
Circle::center() const {
    return -v_(0,1)/v_(0,0);
}

inline double
Circle::radius() const {
    return (std::sqrt(-det(v_))/v_(0,0)).real();
}

} // appollonian

#endif // CIRCLE_HPP
