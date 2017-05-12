#include "apollonian.hpp"

namespace apollonian {

namespace canonical {

const PComplex a0{1.0, 0.0};
const PComplex a1{0.0, 1.0};
const PComplex a2{1.0, 1.0};

const PComplex b0{0.5+0.5i};
const PComplex b1{1.0+1.0i};
const PComplex b2{    1.0i};

/* The transformations that generate the fancy gasket. */

/* These relate a node to a subnode of the same type (A -> A or B -> B).
 * These are all parabolic transformations.
 */
const ApollonianTransformation m0{{a0, a1, a2, a0, b2, b1}, {3, 1, 2, 0}};
const ApollonianTransformation m1{{a0, a1, a2, b2, a1, b0}, {0, 3, 2, 1}};
const ApollonianTransformation m2{{a0, a1, a2, b1, b0, a2}, {0, 1, 3, 2}};

/* These relate a node to a subnode of the other type (A -> B or B ->
 * A).  The first three are elliptical (having order 4) while the last is
 * hyperbolic.
 */
const ApollonianTransformation n0{{a0, a1, a2, b0, a1, a2}, {3, 2, 1, 0}};
const ApollonianTransformation n1{{a0, a1, a2, a0, b1, a2}, {2, 3, 0, 1}};
const ApollonianTransformation n2{{a0, a1, a2, a0, a1, b2}, {1, 0, 3, 2}};
const ApollonianTransformation p{ {a0, a1, a2, b0, b1, b2}, {0, 1, 2, 3}};

using EdgeType = GraphEdge<ApollonianTransformation>;

static constexpr int M0 = static_cast<int>(TransformationId::M0);
static constexpr int M1 = static_cast<int>(TransformationId::M1);
static constexpr int M2 = static_cast<int>(TransformationId::M2);
static constexpr int N0 = static_cast<int>(TransformationId::N0);
static constexpr int N1 = static_cast<int>(TransformationId::N1);
static constexpr int N2 = static_cast<int>(TransformationId::N2);
static constexpr int P = static_cast<int>(TransformationId::P);

const TransformationGraph<2, ApollonianTransformation>
graph{
    /* Edges for node type A (triangle) */
    std::vector<EdgeType>{
        EdgeType{0, M0, m0},
        EdgeType{0, M1, m1},
        EdgeType{0, M2, m2},
        EdgeType{1, P, p},
    },
    /* Edges for node type B (circle) */
    std::vector<GraphEdge<ApollonianTransformation>>{
        EdgeType{1, M0, m0},
        EdgeType{1, M1, m1},
        EdgeType{1, M2, m2},
        EdgeType{0, N0, n0},
        EdgeType{0, N1, n1},
        EdgeType{0, N2, n2},
        EdgeType{0, P, p},
    },
};

/* Upper half plane. */
const Circle
c{0, -1i, 0};

} // canonical

} // apollonian
