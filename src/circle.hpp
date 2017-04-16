#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <cmath>

#include "riemann_sphere.hpp"
#include "mobius.hpp"

namespace apollonian {

class Circle {
public:
    Circle() = default;
    Circle(const Complex& v00, const Complex& v01,
           const Complex& v10, const Complex& v11);
    Circle(const Complex& center, double radius);
    Circle(const Complex& z0, const Complex& z1, const Complex& z2);
    Circle(const Matrix& v);

    Complex center() const;
    double radius() const;

public:
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
        const Complex& z0, const Complex& z1, const Complex& z2)
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
