#include "apollonian.hpp"

namespace apollonian {

const ApollonianState ApollonianState::identity(
        MobiusTransformation::identity,
        Permutation<4>::identity);

const ApollonianState ApollonianState::m0(
        {1,    1i,
         0,  1   },
        {3, 1, 2, 0});

const ApollonianState ApollonianState::m1(
        { 1,    0,
           -1i, 1},
        {0, 3, 2, 1});

const ApollonianState ApollonianState::m2(
        {1.0-1i,     1i,
            -1i, 1.0+1i},
        {0, 1, 3, 2});

const Circle ApollonianState::c0{ 0, -1,
                                 -1,  2};

const Circle ApollonianState::c1{ 2, -1,
                                 -1,  0};

const Circle ApollonianState::c2{0, 1,
                                 1, 0};

const Circle ApollonianState::c3{ 2.0,    -1.0-2i,
                                 -1.0+2i,  2.0   };

} // apollonian
