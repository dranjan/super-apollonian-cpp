#include "apollonian.hpp"

namespace apollonian {

const ApollonianState ApollonianState::identity(
        MobiusTransformation::identity,
        Permutation<4>::identity);

const ApollonianState ApollonianState::m0(
        MobiusTransformation(Complex(1.0),       Complex(0.0, 1.0),
                             Complex(0.0),       Complex(1.0)),
        Permutation<4>::transposition(0, 3));

const ApollonianState ApollonianState::m1(
        MobiusTransformation(Complex(1.0),       Complex(0.0),
                             Complex(0.0, -1.0), Complex(1.0)),
        Permutation<4>::transposition(1, 3));

const ApollonianState ApollonianState::m2(
        MobiusTransformation(Complex(1.0, -1.0), Complex(0.0, 1.0),
                             Complex(0.0, -1.0), Complex(1.0, 1.0)),
        Permutation<4>::transposition(2, 3));

const Circle ApollonianState::c0(Complex( 0.0),      Complex(-1.0),
                                 Complex(-1.0),      Complex( 2.0));

const Circle ApollonianState::c1(Complex( 2.0),      Complex(-1.0),
                                 Complex(-1.0),      Complex( 0.0));

const Circle ApollonianState::c2(Complex( 0.0),      Complex( 1.0),
                                 Complex( 1.0),      Complex( 0.0));

const Circle ApollonianState::c3(Complex( 2.0),      Complex(-1.0, -2.0),
                                 Complex(-1.0, 2.0), Complex( 2.0));

} // apollonian
