#ifndef APOLLONIAN_HPP
#define APOLLONIAN_HPP

#include <algorithm>

#include "circle.hpp"
#include "mobius.hpp"
#include "permutation.hpp"

namespace apollonian {

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

template <typename F>
void
generate_apollonian_triangle(const ApollonianState& initial_state,
                             double threshold, F& callback)
{
    std::vector<ApollonianState> stack;
    stack.push_back(initial_state);

    while (stack.size()) {
        ApollonianState state = stack.back();
        stack.pop_back();
        callback(state);
        if (state.size() >= threshold) {
            stack.push_back(state*ApollonianState::m0);
            stack.push_back(state*ApollonianState::m1);
            stack.push_back(state*ApollonianState::m2);
        }
    }
}

template <typename F>
void
generate_apollonian_gasket(
        const Complex& z0, const Complex& z1, const Complex& z2,
        double threshold, F& callback)
{
    MobiusTransformation m
        = MobiusTransformation::cross_ratio(z0, z1, z2).inverse();
    m.normalize();
    ApollonianState state(m, Permutation<4>::identity);

    callback(state*ApollonianState::m0.inverse());
    callback(state*ApollonianState::m1.inverse());
    callback(state*ApollonianState::m2.inverse());

    generate_apollonian_triangle(state, threshold, callback);
    generate_apollonian_triangle(state*ApollonianState::inv,
                                 threshold, callback);
}

} // apollonian

#endif // APOLLONIAN_HPP
