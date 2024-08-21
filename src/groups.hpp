#ifndef GROUPS_HPP
#define GROUPS_HPP

#include <utility>
#include <tuple>

namespace apollonian {

template <typename G>
class opposite_group {
public:
    template <typename... Args>
    opposite_group(Args&&... args...);

    opposite_group(const opposite_group&) = default;

    static const opposite_group<G> identity;

    opposite_group<G> inverse() const;
    opposite_group<G> operator * (const opposite_group<G>& other) const;

public:
    G g_;
};

template <typename G0, typename G1>
class product_group {
public:
    product_group(const G0& g0, const G1& g1);
    product_group(const product_group&) = default;

    static const product_group<G0, G1> identity;

    product_group<G0, G1> inverse() const;

    product_group<G0, G1>
    operator * (const product_group<G0, G1>& other) const;

public:
    G0 g0_;
    G1 g1_;
};

template <typename G>
template <typename... Args>
opposite_group<G>::opposite_group(Args&&... args...)
    : g_{std::forward<Args>(args)...}
{
}

template <typename G>
const opposite_group<G>
opposite_group<G>::identity{G::identity};

template <typename G>
opposite_group<G>
opposite_group<G>::inverse() const {
    return {g_.inverse()};
}

template <typename G>
opposite_group<G>
opposite_group<G>::operator * (const opposite_group<G>& other) const {
    return {other.g_ * g_};
}

template <typename G0, typename G1>
product_group<G0, G1>::product_group(const G0& g0, const G1& g1)
    : g0_{g0}, g1_{g1}
{
}

template <typename G0, typename G1>
const product_group<G0, G1>
product_group<G0, G1>::identity{G0::identity, G1::identity};

template <typename G0, typename G1>
product_group<G0, G1>
product_group<G0, G1>::inverse() const {
    return {g0_.inverse(), g1_.inverse()};
}

template <typename G0, typename G1>
product_group<G0, G1>
product_group<G0, G1>::operator * (
        const product_group<G0, G1>& other) const
{
    return {g0_*other.g0_, g1_*other.g1_};
}

} // apollonian

#endif // GROUPS_HPP
