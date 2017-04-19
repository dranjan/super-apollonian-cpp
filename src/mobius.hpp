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
class MobiusTransformation {
public:
    MobiusTransformation() = default;
    MobiusTransformation(const Complex& v00, const Complex& v01,
                         const Complex& v10, const Complex& v11);
    MobiusTransformation(
            const PComplex& z0, const PComplex& z1, const PComplex& z2,
            const PComplex& w0, const PComplex& w1, const PComplex& w2);

    MobiusTransformation inverse() const;
    MobiusTransformation operator * (
            const MobiusTransformation& other) const;

    void normalize();

    static MobiusTransformation
    cross_ratio(const PComplex& z0, const PComplex& z1, const PComplex& z2);

    static const MobiusTransformation identity;

    template <typename T>
    T operator () (const T& t) const;

public:
    Complex v00_;
    Complex v01_;
    Complex v10_;
    Complex v11_;
};

inline
MobiusTransformation::MobiusTransformation(
        const Complex& v00, const Complex& v01,
        const Complex& v10, const Complex& v11)
    : v00_{v00}, v01_{v01}, v10_{v10}, v11_{v11}
{
}

inline
MobiusTransformation::MobiusTransformation(
        const PComplex& z0, const PComplex& z1, const PComplex& z2,
        const PComplex& w0, const PComplex& w1, const PComplex& w2)
{
    MobiusTransformation p = cross_ratio(z0, z1, z2);
    MobiusTransformation q = cross_ratio(w0, w1, w2);
    *this = q.inverse()*p;
    normalize();
}

inline MobiusTransformation
MobiusTransformation::inverse() const {
    return { v11_, -v01_,
            -v10_,  v00_};
}

inline MobiusTransformation
MobiusTransformation::operator * (const MobiusTransformation& other) const {
    return MobiusTransformation{
        v00_*other.v00_ + v01_*other.v10_,
        v00_*other.v01_ + v01_*other.v11_,
        v10_*other.v00_ + v11_*other.v10_,
        v10_*other.v01_ + v11_*other.v11_,
    };
}

inline void
MobiusTransformation::normalize() {
    Complex f = std::sqrt(v00_*v11_ - v01_*v10_);
    v00_ *= f;
    v01_ *= f;
    v10_ *= f;
    v11_ *= f;
}

inline MobiusTransformation
MobiusTransformation::cross_ratio(
        const PComplex& z0, const PComplex& z1, const PComplex& z2)
{
    const Complex& a0(z0.v0_);
    const Complex& a1(z1.v0_);
    const Complex& a2(z2.v0_);

    const Complex& b0(z0.v1_);
    const Complex& b1(z1.v1_);
    const Complex& b2(z2.v1_);

    return {b1*(a0*b2 - a2*b0), a1*(a2*b0 - a0*b2),
            b0*(a1*b2 - a2*b1), a0*(a2*b1 - a1*b2)};
}

template <>
inline PComplex
MobiusTransformation::operator () (const PComplex& t) const {
    return {v00_*t.v0_ + v01_*t.v1_,
            v10_*t.v0_ + v11_*t.v1_};
}

template <>
inline Complex
MobiusTransformation::operator () (const Complex& t) const {
    return operator () (PComplex(t));
}

} // apollonian

#endif // MOBIUS_HPP
