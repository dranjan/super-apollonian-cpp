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

    double size(const Complex& z0, const Complex& z1, const Complex& z2) const;

    static const ApollonianState identity;

public:
    MobiusTransformation m_;
    Permutation<4> p_;
};

class ApollonianGenerators {
public:
    ApollonianGenerators(const ApollonianState& m0,
                         const ApollonianState& m1,
                         const ApollonianState& m2,
                         const Circle& c);

    static ApollonianGenerators
    create(const Complex& a0, const Complex& a1, const Complex& a2);

    void get_vertices(Complex& a0, Complex& a1, Complex& a2) const;

    ApollonianGenerators inverse() const;

    template <typename F>
    void iterate_triangle(double threshold, F& callback);

    template <typename F>
    void iterate(double threshold, F& callback);

public:
    ApollonianState m0_;
    ApollonianState m1_;
    ApollonianState m2_;

    Circle c_;
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
ApollonianState::size(
        const Complex& z0, const Complex& z1, const Complex& z2) const
{
    Complex w0 = m_(z0);
    Complex w1 = m_(z1);
    Complex w2 = m_(z2);
    return std::max({std::abs(w0 - w1), std::abs(w1 - w2), std::abs(w2 - w0)});
}

inline
ApollonianGenerators::ApollonianGenerators(const ApollonianState& m0,
                                           const ApollonianState& m1,
                                           const ApollonianState& m2,
                                           const Circle& c)
    : m0_{m0}, m1_{m1}, m2_{m2}, c_{c}
{
}

template <typename F>
void
ApollonianGenerators::iterate_triangle(double threshold, F& callback) {
    Circle c;
    size_t p[4] = {0, 1, 2, 3};
    size_t q[4];

    Complex a0;
    Complex a1;
    Complex a2;
    get_vertices(a0, a1, a2);

    std::vector<ApollonianState> stack;
    stack.push_back(ApollonianState::identity);

    while (stack.size()) {
        ApollonianState state = stack.back();
        stack.pop_back();
        c = state.m_(c_);
        state.p_(p, q);
        callback(c, q);
        if (state.size(a0, a1, a2) >= threshold) {
            stack.push_back(state*m0_);
            stack.push_back(state*m1_);
            stack.push_back(state*m2_);
        }
    }
}

template <typename F>
void
ApollonianGenerators::iterate(double threshold, F& callback) {
    Circle c;
    size_t p[4] = {0, 1, 2, 3};
    size_t q[4];

    ApollonianGenerators inv = inverse();

    c = inv.m0_.m_(c_);
    inv.m0_.p_(p, q);
    callback(c, q);

    c = inv.m1_.m_(c_);
    inv.m1_.p_(p, q);
    callback(c, q);

    c = inv.m2_.m_(c_);
    inv.m2_.p_(p, q);
    callback(c, q);

    iterate_triangle(threshold, callback);
    inv.iterate_triangle(threshold, callback);
}

} // apollonian

#endif // APOLLONIAN_HPP
