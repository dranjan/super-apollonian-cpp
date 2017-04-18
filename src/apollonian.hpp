#ifndef APOLLONIAN_HPP
#define APOLLONIAN_HPP

#include <algorithm>

#include "circle.hpp"
#include "mobius.hpp"
#include "permutation.hpp"

namespace apollonian {

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
    ApollonianState(const MobiusTransformation& m,
                    const Permutation<4>& p);

    ApollonianState inverse() const;
    ApollonianState operator * (const ApollonianState& other) const;

    double size() const;
    operator Circle () const;

    static const ApollonianState identity;

    /* These transformations and disks describe the canonical
     * Apollonian gasket.  An arbitrary Apollonian gasket can be
     * expressed as the image of the canonical gasket under a Mobius
     * transformation, with the transformation being conveniently
     * determined by the tangency points of three of the mutually
     * tangent disks (see generate_apollonian_gasket).
     *
     * See the .cpp file for more details.
     */
    static const ApollonianState m0;
    static const ApollonianState m1;
    static const ApollonianState m2;
    static const ApollonianState inv;

    static const Circle c0;
    static const Circle c1;
    static const Circle c2;
    static const Circle c3;

public:
    MobiusTransformation m_;
    Permutation<4> p_;
};

inline
ApollonianState::ApollonianState(const MobiusTransformation& m,
                                 const Permutation<4>& p)
    : m_{m}, p_{p}
{
}

inline ApollonianState
ApollonianState::inverse() const {
    return ApollonianState(m_.inverse(), p_.inverse());
}

inline ApollonianState
ApollonianState::operator * (const ApollonianState& other) const {
    return ApollonianState(m_*other.m_, other.p_*p_);
}

/* This is an approximation to the diamater of the Apollonian
 * triangle.  It isn't accurate in all cases, but it may be good enough
 * for some uses.
 */
inline double
ApollonianState::size() const
{
    Complex w0 = m_.v_(0,0)/m_.v_(1,0);
    Complex w1 = m_.v_(0,1)/m_.v_(1,1);
    Complex w2 = (m_.v_(0,0) + m_.v_(0,1))/(m_.v_(1,0) + m_.v_(1,1));
    return std::max({std::abs(w0 - w1), std::abs(w1 - w2), std::abs(w2 - w0)});
}

inline
ApollonianState::operator Circle () const {
    return m_(c3);
}

template <typename Visitor>
void
generate_apollonian_triangle(const ApollonianState& initial_state,
                             double threshold, Visitor& visitor)
{
    /* This could equally well be done with explicit recursion, but we
     * use an explicit stack as a more lightweight alternative.
     */
    std::vector<ApollonianState> stack;
    stack.push_back(initial_state);

    while (stack.size()) {
        ApollonianState state = stack.back();
        stack.pop_back();
        visitor(state);
        if (state.size() >= threshold) {
            stack.push_back(state*ApollonianState::m0);
            stack.push_back(state*ApollonianState::m1);
            stack.push_back(state*ApollonianState::m2);
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
 *     void operator () (const ApollonianState& s);
 */
template <typename Visitor>
void
generate_apollonian_gasket(
        const PComplex& z0, const PComplex& z1, const PComplex& z2,
        double threshold, Visitor& visitor)
{
    MobiusTransformation m
        = MobiusTransformation::cross_ratio(z0, z1, z2).inverse();
    m.normalize();
    ApollonianState state(m, Permutation<4>::identity);

    visitor(state*ApollonianState::m0.inverse());
    visitor(state*ApollonianState::m1.inverse());
    visitor(state*ApollonianState::m2.inverse());

    generate_apollonian_triangle(state, threshold, visitor);
    generate_apollonian_triangle(state*ApollonianState::inv,
                                 threshold, visitor);
}

} // apollonian

#endif // APOLLONIAN_HPP
