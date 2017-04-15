#include "apollonian.hpp"

namespace apollonian {

const ApollonianState ApollonianState::identity(
        MobiusTransformation::identity,
        Permutation<4>::identity);

ApollonianGenerators
ApollonianGenerators::create(
        const Complex& a0, const Complex& a1, const Complex& a2)
{
    MobiusTransformation m
        = MobiusTransformation::cross_ratio(a0, a1, a2).inverse();
    m.normalize();

    Complex b0 = m(Complex(0.5, 0.5));
    Complex b1 = m(Complex(1.0, 1.0));
    Complex b2 = m(Complex(0.0, 1.0));

    ApollonianState m0(
            MobiusTransformation(a0, a1, a2,
                                 a0, b2, b1),
            Permutation<4>::transposition(0, 3));

    ApollonianState m1(
            MobiusTransformation(a0, a1, a2,
                                 b2, a1, b0),
            Permutation<4>::transposition(1, 3));

    ApollonianState m2(
            MobiusTransformation(a0, a1, a2,
                                 b1, b0, a2),
            Permutation<4>::transposition(2, 3));

    Circle c(m(Circle{Complex( 2.0),      Complex(-1.0, -2.0),
                      Complex(-1.0, 2.0), Complex( 2.0)}));

    return ApollonianGenerators(m0, m1, m2, c);
}

void
ApollonianGenerators::get_vertices(
        Complex& a0, Complex& a1, Complex& a2) const
{
    MobiusTransformation m0 = m0_.m_.inverse();
    MobiusTransformation m1 = m1_.m_.inverse();
    MobiusTransformation m2 = m2_.m_.inverse();

    Vector v0 = (m1(c_).v_ + m2(c_).v_).row(0);
    Vector v1 = (m2(c_).v_ + m0(c_).v_).row(0);
    Vector v2 = (m0(c_).v_ + m1(c_).v_).row(0);

    a0 = -v0(1)/v0(0);
    a1 = -v1(1)/v1(0);
    a2 = -v2(1)/v2(0);
}

ApollonianGenerators
ApollonianGenerators::inverse() const {
    MobiusTransformation m = m0_.m_.inverse();
    return ApollonianGenerators(m0_.inverse(),
                                m1_.inverse(),
                                m2_.inverse(),
                                m(m(c_)));
}

} // apollonian
