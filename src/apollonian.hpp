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

    template <typename Callback>
    void iterate(Callback&& callback) const;

    double size() const;

    operator Circle() const;

public:

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

template <typename Data>
template <typename Callback>
void
ApollonianState<Data>::iterate(Callback&& callback) const {
    unsigned int index = static_cast<unsigned int>(type_);
    for (const auto& e : canonical::graph.edges_[index]) {
        callback(static_cast<NodeType>(e.type_index), t_*e.transform);
    }
}

template <typename Data>
inline double
ApollonianState<Data>::size() const {
    const auto& m = t_.g0_;
    Complex w0;
    Complex w1;
    Complex w2;

    switch (type_) {
    case NodeType::A:
        w0 = m.v00_/m.v10_;
        w1 = m.v01_/m.v11_;
        w2 = (m.v00_ + m.v01_)/(m.v10_ + m.v11_);
        return std::max({std::abs(w0 - w1),
                         std::abs(w1 - w2),
                         std::abs(w2 - w0)});

    case NodeType::B:
        return m(canonical::c).radius();

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

    /* This could equally well be done with explicit recursion, but we
     * use an explicit stack as a more lightweight alternative.
     */
    std::vector<State> stack;

    MobiusTransformation m
        = MobiusTransformation::cross_ratio(z0, z1, z2).inverse();
    Transform t{m, Permutation<4>::identity};

    /* Involution that swaps the interior and exterior of the main
     * circle.
     */
    Transform inv{{a0, a1, a2, a0, a2, a1}, {0, 2, 1, 3}};

    stack.emplace_back(NodeType::B, t*canonical::p.inverse(), data0);
    stack.emplace_back(NodeType::B, t*canonical::p.inverse()*inv, data1);

    while (stack.size()) {
        State state = stack.back();
        stack.pop_back();
        if (visitor(state)) {
            state.iterate(
                [&stack, &visitor, &state] (
                        const NodeType& type,
                        const ApollonianTransformation& t)
                {
                    stack.emplace_back(type, t,
                                       visitor.transform(state.data_,
                                                         type, t));
                }
            );
        }
    }
}

} // apollonian

#endif // APOLLONIAN_HPP
