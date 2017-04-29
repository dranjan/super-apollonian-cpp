#ifndef APOLLONIAN_HPP
#define APOLLONIAN_HPP

#include <cassert>

#include <vector>
#include <algorithm>

#include "circle.hpp"
#include "mobius.hpp"
#include "permutation.hpp"
#include "groups.hpp"

namespace apollonian {

using ApollonianTransformation
    = ProductGroup<MobiusTransformation,
                   OppositeGroup<Permutation<4>>>;

class ApollonianState {
public:
    enum class NodeType {
        A = 0,  /* triangle-type */
        B = 1,  /* circle-type */
    };

public:
    ApollonianState() = default;
    ApollonianState(NodeType type,
                    const ApollonianTransformation& m);
    ApollonianState(const ApollonianState&) = default;

    template <typename Callback>
    void iterate(Callback&& callback) const;

    double size() const;

    operator Circle() const;

public:
    static const ApollonianTransformation m0;
    static const ApollonianTransformation m1;
    static const ApollonianTransformation m2;

    static const ApollonianTransformation n0;
    static const ApollonianTransformation n1;
    static const ApollonianTransformation n2;

    static const ApollonianTransformation p;

    static const ApollonianTransformation inv;

    static const Circle c;

public:
    NodeType type_;
    ApollonianTransformation t_;
};

inline
ApollonianState::ApollonianState(NodeType type,
                                 const ApollonianTransformation& t)
    : type_{type}, t_{t}
{
}

template <typename Callback>
void
ApollonianState::iterate(Callback&& callback) const {
    switch (type_) {
    case NodeType::A:
        callback(NodeType::A, t_*m0);
        callback(NodeType::A, t_*m1);
        callback(NodeType::A, t_*m2);
        callback(NodeType::B, t_*p);
        break;
    case NodeType::B:
        callback(NodeType::B, t_*m0);
        callback(NodeType::B, t_*m1);
        callback(NodeType::B, t_*m2);
        callback(NodeType::A, t_*n0);
        callback(NodeType::A, t_*n1);
        callback(NodeType::A, t_*n2);
        callback(NodeType::A, t_*p);
        break;
    default:
        assert(false);
    }
}

inline double
ApollonianState::size() const {
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
        return m(c).radius();

    default:
        assert(false);
    }

    return 0;
}

inline
ApollonianState::operator Circle () const {
    return t_.g0_(c);
}

template <typename Visitor>
void
generate_apollonian_gasket(
        const PComplex& z0, const PComplex& z1, const PComplex& z2,
        Visitor& visitor)
{
    using State = ApollonianState;
    using Transform = ApollonianTransformation;
    using NodeType = State::NodeType;

    /* This could equally well be done with explicit recursion, but we
     * use an explicit stack as a more lightweight alternative.
     */
    std::vector<State> stack;

    MobiusTransformation m
        = MobiusTransformation::cross_ratio(z0, z1, z2).inverse();
    Transform tp{m, Permutation<4>::identity};
    Transform t = tp*State::p.inverse();

    stack.emplace_back(NodeType::A, tp);
    stack.emplace_back(NodeType::A, tp*State::inv);

    stack.emplace_back(NodeType::B, t*State::m0);
    stack.emplace_back(NodeType::B, t*State::m1);
    stack.emplace_back(NodeType::B, t*State::m2);

    while (stack.size()) {
        ApollonianState state = stack.back();
        stack.pop_back();
        if (visitor(state)) {
            state.iterate(
                [&stack] (const ApollonianState::NodeType& type,
                          const ApollonianTransformation& t)
                {
                    stack.emplace_back(type, t);
                }
            );
        }
    }
}

} // apollonian

#endif // APOLLONIAN_HPP
