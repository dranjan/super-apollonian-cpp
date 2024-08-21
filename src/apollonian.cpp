#include "apollonian.hpp"

namespace apollonian {

namespace canonical {

const pcomplex a0{1.0, 0.0};
const pcomplex a1{0.0, 1.0};
const pcomplex a2{1.0, 1.0};

const pcomplex b0{0.5+0.5i};
const pcomplex b1{1.0+1.0i};
const pcomplex b2{    1.0i};

/* The transformations that generate the fancy gasket. */

/* These relate a node to a subnode of the same type (A -> A or B -> B).
 * These are all parabolic transformations.
 */
const apollonian_transformation m0{{a0, a1, a2, a0, b2, b1}, {3, 1, 2, 0}};
const apollonian_transformation m1{{a0, a1, a2, b2, a1, b0}, {0, 3, 2, 1}};
const apollonian_transformation m2{{a0, a1, a2, b1, b0, a2}, {0, 1, 3, 2}};

/* These relate a node to a subnode of the other type (A -> B or B ->
 * A).  The first three are elliptical (having order 4) while the last is
 * hyperbolic.
 */
const apollonian_transformation n0{{a0, a1, a2, b0, a1, a2}, {3, 2, 1, 0}};
const apollonian_transformation n1{{a0, a1, a2, a0, b1, a2}, {2, 3, 0, 1}};
const apollonian_transformation n2{{a0, a1, a2, a0, a1, b2}, {1, 0, 3, 2}};
const apollonian_transformation p{ {a0, a1, a2, b0, b1, b2}, {0, 1, 2, 3}};

using edge_type = graph_edge<apollonian_transformation>;

static constexpr int M0 = static_cast<int>(transformation_id::M0);
static constexpr int M1 = static_cast<int>(transformation_id::M1);
static constexpr int M2 = static_cast<int>(transformation_id::M2);
static constexpr int N0 = static_cast<int>(transformation_id::N0);
static constexpr int N1 = static_cast<int>(transformation_id::N1);
static constexpr int N2 = static_cast<int>(transformation_id::N2);
static constexpr int P = static_cast<int>(transformation_id::P);

const transformation_graph<2, apollonian_transformation>
graph{
    /* Edges for node type A (triangle) */
    std::vector<edge_type>{
        edge_type{0, M0, m0},
        edge_type{0, M1, m1},
        edge_type{0, M2, m2},
        edge_type{1, P, p},
    },
    /* Edges for node type B (circle) */
    std::vector<graph_edge<apollonian_transformation>>{
        edge_type{1, M0, m0},
        edge_type{1, M1, m1},
        edge_type{1, M2, m2},
        edge_type{0, N0, n0},
        edge_type{0, N1, n1},
        edge_type{0, N2, n2},
        edge_type{0, P, p},
    },
};

/* Upper half plane. */
const circle c{0, -1i, 0};

} // canonical

} // apollonian
