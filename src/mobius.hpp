/* SPDX-License-Identifier: GPL-3.0-only
 *
 * Copyright 2024 Darsh Ranjan.
 *
 * This file is part of super-apollonian-cpp.
 */

#ifndef MOBIUS_HPP
#define MOBIUS_HPP

#include "riemann_sphere.hpp"

namespace apollonian {

/* A Mobius transformation is an invertible linear transformation of the
 * projective complex plane.  In non-projective terms, it is a linear
 * fractional transformation, i.e.,
 *
 *     z -> (a*z + b)/(c*z + d)
 *
 * for some a, b, c, and d.  The 2 by 2 matrix representation, which
 * follows immediately from the projective definition, is very
 * convenient.
 */
class mobius_transformation {
public:
    mobius_transformation() = default;
    mobius_transformation(const dcomplex& v00, const dcomplex& v01,
                          const dcomplex& v10, const dcomplex& v11);
    mobius_transformation(
            const pcomplex& z0, const pcomplex& z1, const pcomplex& z2,
            const pcomplex& w0, const pcomplex& w1, const pcomplex& w2);

    mobius_transformation inverse() const;
    mobius_transformation operator * (
            const mobius_transformation& other) const;

    void normalize();

    static mobius_transformation
    cross_ratio(const pcomplex& z0, const pcomplex& z1, const pcomplex& z2);

    static const mobius_transformation identity;

    template <typename T>
    T operator () (const T& t) const;

public:
    dcomplex v00_;
    dcomplex v01_;
    dcomplex v10_;
    dcomplex v11_;
};

inline
mobius_transformation::mobius_transformation(
        const dcomplex& v00, const dcomplex& v01,
        const dcomplex& v10, const dcomplex& v11)
    : v00_{v00}, v01_{v01}, v10_{v10}, v11_{v11}
{
}

inline
mobius_transformation::mobius_transformation(
        const pcomplex& z0, const pcomplex& z1, const pcomplex& z2,
        const pcomplex& w0, const pcomplex& w1, const pcomplex& w2)
{
    mobius_transformation p = cross_ratio(z0, z1, z2);
    mobius_transformation q = cross_ratio(w0, w1, w2);
    *this = q.inverse()*p;
}

inline mobius_transformation
mobius_transformation::inverse() const {
    return { v11_, -v01_,
            -v10_,  v00_};
}

inline mobius_transformation
mobius_transformation::operator * (const mobius_transformation& other) const {
    return mobius_transformation{
        v00_*other.v00_ + v01_*other.v10_,
        v00_*other.v01_ + v01_*other.v11_,
        v10_*other.v00_ + v11_*other.v10_,
        v10_*other.v01_ + v11_*other.v11_,
    };
}

inline void
mobius_transformation::normalize() {
    dcomplex f = std::sqrt(v00_*v11_ - v01_*v10_);
    v00_ *= f;
    v01_ *= f;
    v10_ *= f;
    v11_ *= f;
}

inline mobius_transformation
mobius_transformation::cross_ratio(
        const pcomplex& z0, const pcomplex& z1, const pcomplex& z2)
{
    const dcomplex& a0(z0.v0_);
    const dcomplex& a1(z1.v0_);
    const dcomplex& a2(z2.v0_);

    const dcomplex& b0(z0.v1_);
    const dcomplex& b1(z1.v1_);
    const dcomplex& b2(z2.v1_);

    dcomplex det02 = a0*b2 - a2*b0;
    dcomplex det21 = a2*b1 - a1*b2;
    dcomplex det10 = a1*b0 - a0*b1;

    dcomplex f = 1.0/std::sqrt(det02*det21*det10);
    dcomplex num = det02*f;
    dcomplex den = det21*f;

    return { b1*num, -a1*num,
            -b0*den,  a0*den};
}

template <>
inline pcomplex
mobius_transformation::operator () (const pcomplex& t) const {
    return {v00_*t.v0_ + v01_*t.v1_,
            v10_*t.v0_ + v11_*t.v1_};
}

template <>
inline dcomplex
mobius_transformation::operator () (const dcomplex& t) const {
    return operator () (pcomplex(t));
}

} // apollonian

#endif // MOBIUS_HPP
