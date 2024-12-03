/* SPDX-License-Identifier: GPL-3.0-only
 *
 * Copyright 2024 Darsh Ranjan.
 *
 * This file is part of super-apollonian-cpp.
 */

#ifndef RIEMANN_SPHERE_HPP
#define RIEMANN_SPHERE_HPP

#include <complex>

namespace apollonian {

using dcomplex = std::complex<double>;

using namespace std::complex_literals;

/* Projective complex number */
class pcomplex {
public:
    pcomplex() = default;
    pcomplex(const dcomplex& numerator,
             const dcomplex& denominator);
    pcomplex(const dcomplex& value);

    operator dcomplex() const;

public:
    dcomplex v0_;
    dcomplex v1_;
};

inline
pcomplex::pcomplex(const dcomplex& numerator,
                   const dcomplex& denominator)
    : v0_{numerator}, v1_{denominator}
{
}

inline
pcomplex::pcomplex(const dcomplex& value) {
    if (std::isinf(value.real()) || std::isinf(value.imag())) {
        v0_ = 1;
        v1_ = 0;
    } else {
        v0_ = value;
        v1_ = 1;
    }
}

inline
pcomplex::operator dcomplex() const {
    return v0_/v1_;
}

} // apollonian

#endif // RIEMANN_SPHERE_HPP
