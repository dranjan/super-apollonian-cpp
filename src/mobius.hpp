#ifndef MOBIUS_HPP
#define MOBIUS_HPP

#include "riemann_sphere.hpp"

namespace apollonian {

class MobiusTransformation {
public:
    MobiusTransformation() = default;
    MobiusTransformation(const Complex& v00, const Complex& v01,
                         const Complex& v10, const Complex& v11);
    MobiusTransformation(const Matrix& v);
    MobiusTransformation(
            const Complex& z0, const Complex& z1, const Complex& z2,
            const Complex& w0, const Complex& w1, const Complex& w2);

    MobiusTransformation inverse() const;
    MobiusTransformation operator * (
            const MobiusTransformation& other) const;

    void normalize();

    static MobiusTransformation
    cross_ratio(const Complex& z0, const Complex& z1, const Complex& z2);

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
        const Complex& z0, const Complex& z1, const Complex& z2,
        const Complex& w0, const Complex& w1, const Complex& w2)
{
    MobiusTransformation p = cross_ratio(z0, z1, z2);
    MobiusTransformation q = cross_ratio(w0, w1, w2);
    *this = q.inverse()*p;
    normalize();
}

inline MobiusTransformation
MobiusTransformation::inverse() const {
    return MobiusTransformation( v_(1,1), -v_(0,1),
                                -v_(1,0),  v_(0,0));
}

inline MobiusTransformation
MobiusTransformation::operator * (const MobiusTransformation& other) const {
    return MobiusTransformation(v_ * other.v_);
}

inline void
MobiusTransformation::normalize() {
    v_ /= std::sqrt(det(v_));
}

inline MobiusTransformation
MobiusTransformation::cross_ratio(
        const Complex& z0, const Complex& z1, const Complex& z2)
{
    return MobiusTransformation(z0 - z2, z1*(z2 - z0),
                                z1 - z2, z0*(z2 - z1));
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
