#ifndef RIEMANN_SPHERE_HPP
#define RIEMANN_SPHERE_HPP

#include <complex>

#include <cmath>

#include <Eigen/Core>

namespace apollonian {

using Complex = std::complex<double>;
using Matrix = Eigen::Matrix2cd;
using Vector = Eigen::Vector2cd;

using namespace std::complex_literals;

/* Projective complex number */
class PComplex {
public:
    PComplex() = default;
    PComplex(const Complex& numerator,
             const Complex& denominator);
    PComplex(const Complex& value);
    PComplex(const Vector& v);

    operator Complex() const;

public:
    Vector v_;  /* {numerator, denominator} */
};

inline
PComplex::PComplex(const Complex& numerator,
                   const Complex& denominator)
    : v_{numerator, denominator}
{
}

inline
PComplex::PComplex(const Complex& value) {
    if (std::isinf(value.real()) || std::isinf(value.imag())) {
        v_(0) = 1;
        v_(1) = 0;
    } else {
        v_(0) = value;
        v_(1) = 1;
    }
}

inline
PComplex::PComplex(const Vector& v)
    : v_{v}
{
}

inline
PComplex::operator Complex() const {
    return v_(0)/v_(1);
}

inline Complex
det(const Matrix& v) {
    return v(0,0)*v(1,1) - v(0,1)*v(1,0);
}

} // apollonian

#endif // RIEMANN_SPHERE_HPP
