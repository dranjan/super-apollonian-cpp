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

namespace canonical {
    extern const ApollonianTransformation m0;
    extern const ApollonianTransformation m1;
    extern const ApollonianTransformation m2;

    extern const ApollonianTransformation n0;
    extern const ApollonianTransformation n1;
    extern const ApollonianTransformation n2;

    extern const ApollonianTransformation p;

    extern const ApollonianTransformation inv;

    extern const Circle c;

} // canonical

enum class NodeType {
    A = 0,  /* triangle-type */
    B = 1,  /* circle-type */
};

template <typename Data>
class ApollonianState {
public:

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
    using namespace canonical;

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
    using namespace canonical;

    /* This could equally well be done with explicit recursion, but we
     * use an explicit stack as a more lightweight alternative.
     */
    std::vector<State> stack;

    MobiusTransformation m
        = MobiusTransformation::cross_ratio(z0, z1, z2).inverse();
    Transform t{m, Permutation<4>::identity};

    stack.emplace_back(NodeType::B, t*p.inverse(), data0);
    stack.emplace_back(NodeType::B, t*p.inverse()*inv, data1);

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
