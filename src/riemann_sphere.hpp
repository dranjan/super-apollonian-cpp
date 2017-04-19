#ifndef RIEMANN_SPHERE_HPP
#define RIEMANN_SPHERE_HPP

#include <complex>

namespace apollonian {

using Complex = std::complex<double>;

using namespace std::complex_literals;

/* Projective complex number */
class PComplex {
public:
    PComplex() = default;
    PComplex(const Complex& numerator,
             const Complex& denominator);
    PComplex(const Complex& value);

    operator Complex() const;

public:
    Complex v0_;
    Complex v1_;
};

inline
PComplex::PComplex(const Complex& numerator,
                   const Complex& denominator)
    : v0_{numerator}, v1_{denominator}
{
}

inline
PComplex::PComplex(const Complex& value) {
    if (std::isinf(value.real()) || std::isinf(value.imag())) {
        v0_ = 1;
        v1_ = 0;
    } else {
        v0_ = value;
        v1_ = 1;
    }
}

inline
PComplex::operator Complex() const {
    return v0_/v1_;
}

} // apollonian

#endif // RIEMANN_SPHERE_HPP
