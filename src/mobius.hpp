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
    MobiusTransformation(const Matrix& v);
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
    Matrix v_;
};

inline
MobiusTransformation::MobiusTransformation(
        const Complex& v00, const Complex& v01,
        const Complex& v10, const Complex& v11)
{
    v_ << v00, v01,
          v10, v11;
}
inline
MobiusTransformation::MobiusTransformation(const Matrix& v)
    : v_{v}
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
    return MobiusTransformation{ v_(1,1), -v_(0,1),
                                -v_(1,0),  v_(0,0)};
}

inline MobiusTransformation
MobiusTransformation::operator * (const MobiusTransformation& other) const {
    return MobiusTransformation{v_ * other.v_};
}

inline void
MobiusTransformation::normalize() {
    v_ /= std::sqrt(det(v_));
}

inline MobiusTransformation
MobiusTransformation::cross_ratio(
        const PComplex& z0, const PComplex& z1, const PComplex& z2)
{
    const Complex& a0(z0.v_(0));
    const Complex& a1(z1.v_(0));
    const Complex& a2(z2.v_(0));

    const Complex& b0(z0.v_(1));
    const Complex& b1(z1.v_(1));
    const Complex& b2(z2.v_(1));

    return MobiusTransformation{b1*(a0*b2 - a2*b0), a1*(a2*b0 - a0*b2),
                                b0*(a1*b2 - a2*b1), a0*(a2*b1 - a1*b2)};
}

template <>
inline PComplex
MobiusTransformation::operator () (const PComplex& t) const {
    return PComplex(v_*t.v_);
}

template <>
inline Complex
MobiusTransformation::operator () (const Complex& t) const {
    return operator () (PComplex(t));
}

} // apollonian

#endif // MOBIUS_HPP
