#include "apollonian.hpp"

namespace apollonian {

namespace {

/* These are the six tangency points involving the circles in the
 * "canonical" Apollonian gasket.
 */

/* These are the three tangency points not involving the fourth circle.
 */
PComplex a0{1.0, 0.0};
PComplex a1{0.0, 1.0};
PComplex a2{1.0, 1.0};

/* These are the three tangency points involving the fourth circle. */
PComplex b0{0.5+0.5i};
PComplex b1{1.0+1.0i};
PComplex b2{    1.0i};

}

/* These are the transformations that generate the canonical "super"
 * Apollonian gasket (as a monoid).
 */
const ApollonianTransformation
ApollonianState::m0{ {a0, a1, a2, a0, b2, b1},
                     {3, 1, 2, 0}};

const ApollonianTransformation
ApollonianState::m1{ {a0, a1, a2, b2, a1, b0},
                     {0, 3, 2, 1}};

const ApollonianTransformation
ApollonianState::m2{ {a0, a1, a2, b1, b0, a2},
                     {0, 1, 3, 2}};

const ApollonianTransformation
ApollonianState::n0{ {a0, a1, a2, b0, a1, a2},
                     {3, 2, 1, 0}};

const ApollonianTransformation
ApollonianState::n1{ {a0, a1, a2, a0, b1, a2},
                     {2, 3, 0, 1}};

const ApollonianTransformation
ApollonianState::n2{ {a0, a1, a2, a0, a1, b2},
                     {1, 0, 3, 2}};

const ApollonianTransformation
ApollonianState::p{  {a0, a1, a2, b0, b1, b2},
                     {0, 1, 2, 3}};

const ApollonianTransformation
ApollonianState::inv{{a0, a1, a2, a0, a2, a1},
                     {0, 2, 1, 3}};

/* Upper half plane. */
const Circle
ApollonianState::c{0, -1i, 0};

} // apollonian
