#ifndef APOLLONIAN_HPP
#define APOLLONIAN_HPP

#include <cassert>

#include <vector>
#include <algorithm>

#include "circle.hpp"
#include "mobius.hpp"
#include "permutation.hpp"
#include "groups.hpp"
#include "transformation_graph.hpp"

namespace apollonian {

using apollonian_transformation
    = product_group<mobius_transformation,
                    opposite_group<permutation<4>>>;

namespace canonical {

enum class transformation_id {
    M0,
    M1,
    M2,
    N0,
    N1,
    N2,
    P,
};

/* These are the six tangency points involving the four circles in the
 * "canonical" Apollonian gasket.
 */

/* The three tangency points not involving the fourth circle. */
extern const pcomplex a0;
extern const pcomplex a1;
extern const pcomplex a2;

/* The three tangency points involving the fourth circle. */
extern const pcomplex b0;
extern const pcomplex b1;
extern const pcomplex b2;

/* These relate a node to a subnode of the same type (A -> A or B -> B).
 */
extern const apollonian_transformation m0;
extern const apollonian_transformation m1;
extern const apollonian_transformation m2;

/* These relate a node to a subnode of the other type (A -> B or B ->
 * A).
 */
extern const apollonian_transformation n0;
extern const apollonian_transformation n1;
extern const apollonian_transformation n2;
extern const apollonian_transformation p;

/* The transformation graph that generates the gasket itself.  This
 * gasket is a fancier version in which the circles are also filled
 * recursively.
 */
extern const transformation_graph<2, apollonian_transformation> graph;

/* Circle through a0, a1, and a2. This is the main circle of the
 * canonical gasket.
 */
extern const circle c;

} // canonical

enum class node_type {
    A = 0,  /* triangle-type */
    B = 1,  /* circle-type */
};

template <typename Data>
class apollonian_state {
    using transform = apollonian_transformation;

public:
    apollonian_state() = default;
    apollonian_state(node_type type,
                     const apollonian_transformation& m,
                     const Data& data);
    apollonian_state(const apollonian_state&) = default;

    /* For a type-A node (triangle), the size is a rough approximation
     * to the diameter.  For a type-B node (circle), the size is the
     * diameter of the circle.
     */
    double size() const;

    /* For a type-A node (triangle), this is the circumcircle of the
     * three vertices.  For a type-B node (circle), this is the circle
     * itself.
     */
    operator circle() const;

public:
    const node_type type_;
    const apollonian_transformation t_;
    const Data data_;
};

template <typename Data>
inline
apollonian_state<Data>::apollonian_state(node_type type,
                                         const apollonian_transformation& t,
                                         const Data& data)
    : type_{type}, t_{t}, data_{data}
{
}

template <typename Data>
inline double
apollonian_state<Data>::size() const {
    auto c = t_.g0_(canonical::c);
    if (c.v00_ <= 0.0) return HUGE_VAL;
    return std::abs(2*c.radius());
}

template <typename Data>
inline
apollonian_state<Data>::operator circle () const {
    return t_.g0_(canonical::c);
}

/* Main entry point to this module.
 *
 * z0, z1, and z2 are the three points on the main circle tangent to
 * each of the respective main sub-circles.
 *
 * data0 and data1 are the initial data attached respectively to the
 * two sides of the main circle.  If z0, z1, and z2 are ordered
 * counterclockwise around the circle, then data0 is interior and data1
 * is exterior, being swapped if the points are ordered clockwise.
 *
 * The Visitor type should have the methods
 *
 *     bool Visitor::visit_node(const apollonian_state<Data>& state);
 *     Data Visitor::get_data(const apollonian_state<Data>& parent,
 *                            node_type type,
 *                            transformation_id id,
 *                            const apollonian_transformation t) const;
 *
 * The return value of visit_node indicates whether we are interested
 * in further iterations of this node.  visit_node will be called once
 * for each generated node (triangle and circle).  The order of nodes
 * is unspecified, but a given node will always be visited before its
 * children.
 *
 * get_data should return the child node's data given the parent node
 * and child node's type and transformation.
 */
template <typename Data, typename Visitor>
void
generate_apollonian_gasket(
        const pcomplex& z0, const pcomplex& z1, const pcomplex& z2,
        const Data& data0, const Data& data1,
        Visitor& visitor)
{
    using State = apollonian_state<Data>;
    using transform = apollonian_transformation;
    using canonical::a0;
    using canonical::a1;
    using canonical::a2;

    /* This could equally well be done with true recursion, but we
     * use an explicit stack as a more lightweight alternative.
     */
    std::vector<State> stack;

    transform t0{{a0, a1, a2, z0, z1, z2}, {0, 1, 2, 3}};
    transform t1{{a0, a1, a2, z0, z2, z1}, {0, 2, 1, 3}};

    /* The two seeds, namely the interior and exterior of the main
     * circle.
     */
    stack.emplace_back(node_type::B, t0, data0);
    stack.emplace_back(node_type::B, t1, data1);

    while (stack.size()) {
        State state = stack.back();
        stack.pop_back();
        if (visitor.visit_node(state)) {
            unsigned int index = static_cast<unsigned int>(state.type_);
            for (const auto& edge : canonical::graph.edges_[index]) {
                node_type type = static_cast<node_type>(edge.type_index);
                canonical::transformation_id id =
                    static_cast<canonical::transformation_id>(edge.id);
                auto t = state.t_*edge.transform;
                stack.emplace_back(type, t,
                                   visitor.get_data(state, type, id, t));
            }
        }
    }
}

} // apollonian

#endif // APOLLONIAN_HPP
