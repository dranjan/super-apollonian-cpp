#ifndef APOLLONIAN_HPP
#define APOLLONIAN_HPP

#include <algorithm>

#include "circle.hpp"
#include "mobius.hpp"
#include "permutation.hpp"
#include "groups.hpp"

namespace apollonian {

using ApollonianTransformation
    = ProductGroup<MobiusTransformation,
                   OppositeGroup<Permutation<4>>>;

/* An "Apollonian state" describes a configuration of four mutually
 * tangent circles (or disks with disjoint interiors) that may be
 * encountered while recursively generating an Apollonian gasket.  More
 * simply, it describes a single "new" circle.
 *
 * It is represented here as a Mobius transformation that transforms the
 * "canonical" four-circle configuration to the current one.
 *
 * Each state also contains a permutation on four elements.  If the
 * disks comprising the complement of the Apollonian gasket are labeled
 * with four colors such that no two tangent circles have the same
 * label, then this permutation maps the labels of the four starting
 * circles to the current four-circle configuration.
 */
class ApollonianState {
public:
    ApollonianState() = default;
    ApollonianState(const ApollonianTransformation& t);
    ApollonianState(const MobiusTransformation& m,
                    const Permutation<4>& p);

    double size() const;
    operator Circle () const;

    /* These transformations and disks describe the canonical
     * Apollonian gasket.  An arbitrary Apollonian gasket can be
     * expressed as the image of the canonical gasket under a Mobius
     * transformation, with the transformation being conveniently
     * determined by the tangency points of three of the mutually
     * tangent disks (see generate_apollonian_gasket).
     *
     * See the .cpp file for more details.
     */
    static const ApollonianTransformation m0;
    static const ApollonianTransformation m1;
    static const ApollonianTransformation m2;
    static const ApollonianTransformation inv;

    static const Circle c0;
    static const Circle c1;
    static const Circle c2;
    static const Circle c3;

public:
    ApollonianTransformation t_;
};

inline
ApollonianState::ApollonianState(const ApollonianTransformation& t)
    : t_{t}
{
}

inline
ApollonianState::ApollonianState(const MobiusTransformation& m,
                                 const Permutation<4>& p)
    : t_{m, p}
{
}

/* This is an approximation to the diamater of the Apollonian
 * triangle.  It isn't accurate in all cases, but it may be good enough
 * for some uses.
 */
inline double
ApollonianState::size() const
{
    const auto& m = t_.g0_;

    Complex w0 = m.v00_/m.v10_;
    Complex w1 = m.v01_/m.v11_;
    Complex w2 = (m.v00_ + m.v01_)/(m.v10_ + m.v11_);
    return std::max({std::abs(w0 - w1),
                     std::abs(w1 - w2),
                     std::abs(w2 - w0)});
}

inline
ApollonianState::operator Circle () const {
    return t_.g0_(c3);
}

template <typename Visitor>
void
generate_apollonian_triangle(const ApollonianState& initial_state,
                             Visitor& visitor)
{
    /* This could equally well be done with explicit recursion, but we
     * use an explicit stack as a more lightweight alternative.
     */
    std::vector<ApollonianState> stack;
    stack.emplace_back(initial_state);

    while (stack.size()) {
        ApollonianState state = stack.back();
        stack.pop_back();
        if (visitor(state)) {
            stack.emplace_back(state.t_*ApollonianState::m0);
            stack.emplace_back(state.t_*ApollonianState::m1);
            stack.emplace_back(state.t_*ApollonianState::m2);
        }
    }
}

/* This is the primary entry point to this module.
 *
 * `z0', `z1', and `z2' are the tangency points of the three mutually
 * tangent circles that intuitively start the iteration.
 *
 * `threshold' is (*very* approximately) how small the "Apollonian
 * triangle" corresponding to a state can get before we stop iterating.
 * The Apollonian triangle for a state is the image of the canonical
 * triangle under the corresponding transformation, where the canonical
 * triangle is the intersection of the full gasket with the complement
 * of the four canonical disks and the upper half plane.
 *
 * `visitor' is a callable that is to be called once on each disk in
 * the complement.  The order of the disks is unspecified.  The visitor
 * should be callable as
 *
 *     bool operator () (const ApollonianState& s);
 *
 * The return value is whether we are interested in further iterations
 * of this state.
 */
template <typename Visitor>
void
generate_apollonian_gasket(
        const PComplex& z0, const PComplex& z1, const PComplex& z2,
        Visitor& visitor)
{
    MobiusTransformation m
        = MobiusTransformation::cross_ratio(z0, z1, z2).inverse();
    ApollonianState state(m, Permutation<4>::identity);

    visitor(state.t_*ApollonianState::m0.inverse());
    visitor(state.t_*ApollonianState::m1.inverse());
    visitor(state.t_*ApollonianState::m2.inverse());

    generate_apollonian_triangle(state, visitor);
    generate_apollonian_triangle(state.t_*ApollonianState::inv, visitor);
}

} // apollonian

#endif // APOLLONIAN_HPP
