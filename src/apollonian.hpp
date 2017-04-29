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

using ApollonianTransformation
    = ProductGroup<MobiusTransformation,
                   OppositeGroup<Permutation<4>>>;

namespace canonical {

/* These are the six tangency points involving the four circles in the
 * "canonical" Apollonian gasket.
 */

/* The three tangency points not involving the fourth circle. */
extern const PComplex a0;
extern const PComplex a1;
extern const PComplex a2;

/* The three tangency points involving the fourth circle. */
extern const PComplex b0;
extern const PComplex b1;
extern const PComplex b2;

/* These relate a node to a subnode of the same type (A -> A or B -> B).
 */
extern const ApollonianTransformation m0;
extern const ApollonianTransformation m1;
extern const ApollonianTransformation m2;

/* These relate a node to a subnode of the other type (A -> B or B ->
 * A).
 */
extern const ApollonianTransformation n0;
extern const ApollonianTransformation n1;
extern const ApollonianTransformation n2;
extern const ApollonianTransformation p;

/* The transformation graph that generates the gasket itself.  This
 * gasket is a fancier version in which the circles are also filled
 * recursively.
 */
extern const TransformationGraph<2, ApollonianTransformation> graph;

/* Circle through a0, a1, and a2. */
extern const Circle c;

} // canonical

enum class NodeType {
    A = 0,  /* triangle-type */
    B = 1,  /* circle-type */
};

template <typename Data>
class ApollonianState {
    using Transform = ApollonianTransformation;

public:
    ApollonianState() = default;
    ApollonianState(NodeType type,
                    const ApollonianTransformation& m,
                    const Data& data);
    ApollonianState(const ApollonianState&) = default;

    /* For a type-A node (triangle), the size is a rough approximation
     * to the diameter.  For a type-B node (circle), the size is the
     * diameter of the circle.
     */
    double size() const;

    /* For a type-A node (triangle), this is the circumcircle of the
     * three vertices.  For a type-B node (circle), this is the circle
     * itself.
     */
    operator Circle() const;

public:
    NodeType type_;
    ApollonianTransformation t_;
    Data data_;
};

template <typename Data>
inline
ApollonianState<Data>::ApollonianState(NodeType type,
                                       const ApollonianTransformation& t,
                                       const Data& data)
    : type_{type}, t_{t}, data_{data}
{
}

inline double
measure_node_a(const MobiusTransformation& m) {
    Complex w0 = m.v00_/m.v10_;
    Complex w1 = m.v01_/m.v11_;
    Complex w2 = (m.v00_ + m.v01_)/(m.v10_ + m.v11_);
    return std::max({std::abs(w0 - w1),
                     std::abs(w1 - w2),
                     std::abs(w2 - w0)});
}

inline double
measure_node_b(const MobiusTransformation& m) {
    return 2*m(canonical::c).radius();
}

template <typename Data>
inline double
ApollonianState<Data>::size() const {
    const auto& m = t_.g0_;
    switch (type_) {
    case NodeType::A:
        return measure_node_a(m);
    case NodeType::B:
        return measure_node_b(m);
    default:
        assert(false);
    }

    return 0;
}

template <typename Data>
inline
ApollonianState<Data>::operator Circle () const {
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
 *     bool Visitor::visit_node(const ApollonianState<Data>&);
 *     Data Visitor::transform_data(const Data&,
 *                                  const NodeType&,
 *                                  const ApollonianTransformation&) const;
 *
 * The return value of visit_node indicates whether we are interested
 * in further iterations of this node.  visit_node will be called once
 * for each generated node (triangle and circle).  The order of nodes
 * is unspecified, but a given node will always be visited before its
 * children.
 *
 * transform_data should return the data to be assigned to a child node
 * given the parent node's data, the child node's type, and the
 * transformation relating the parent and child nodes.
 */
template <typename Data, typename Visitor>
void
generate_apollonian_gasket(
        const PComplex& z0, const PComplex& z1, const PComplex& z2,
        const Data& data0, const Data& data1,
        Visitor& visitor)
{
    using State = ApollonianState<Data>;
    using Transform = ApollonianTransformation;
    using canonical::a0;
    using canonical::a1;
    using canonical::a2;

    /* This could equally well be done with true recursion, but we
     * use an explicit stack as a more lightweight alternative.
     */
    std::vector<State> stack;

    Transform t0{{a0, a1, a2, z0, z1, z2}, {0, 1, 2, 3}};
    Transform t1{{a0, a1, a2, z0, z2, z1}, {0, 2, 1, 3}};

    stack.emplace_back(NodeType::B, t0, data0);
    stack.emplace_back(NodeType::B, t1, data1);

    while (stack.size()) {
        State state = stack.back();
        stack.pop_back();
        if (visitor.visit_node(state)) {
            unsigned int index = static_cast<unsigned int>(state.type_);
            for (const auto& edge : canonical::graph.edges_[index]) {
                NodeType type = static_cast<NodeType>(edge.type_index);
                stack.emplace_back(type, state.t_*edge.transform,
                                   visitor.transform_data(
                                           state.data_,
                                           type, edge.transform));
            }
        }
    }
}

} // apollonian

#endif // APOLLONIAN_HPP
