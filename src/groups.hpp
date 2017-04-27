#ifndef GROUPS_HPP
#define GROUPS_HPP

#include <utility>
#include <tuple>

namespace apollonian {

template <typename G>
class OppositeGroup {
public:
    template <typename... Args>
    OppositeGroup(Args&&... args...);

    OppositeGroup(const OppositeGroup&) = default;

    static const OppositeGroup<G> identity;

    OppositeGroup<G> inverse() const;
    OppositeGroup<G> operator * (const OppositeGroup<G>& other) const;

public:
    G g_;
};

template <typename G0, typename G1>
class ProductGroup {
public:
    ProductGroup(const G0& g0, const G1& g1);
    ProductGroup(const ProductGroup&) = default;

    static const ProductGroup<G0, G1> identity;

    ProductGroup<G0, G1> inverse() const;

    ProductGroup<G0, G1>
    operator * (const ProductGroup<G0, G1>& other) const;

public:
    G0 g0_;
    G1 g1_;
};

template <typename G>
template <typename... Args>
OppositeGroup<G>::OppositeGroup(Args&&... args...)
    : g_{std::forward<Args>(args)...}
{
}

template <typename G>
const OppositeGroup<G>
OppositeGroup<G>::identity{G::identity};

template <typename G>
OppositeGroup<G>
OppositeGroup<G>::inverse() const {
    return {g_.inverse()};
}

template <typename G>
OppositeGroup<G>
OppositeGroup<G>::operator * (const OppositeGroup<G>& other) const {
    return {other.g_ * g_};
}

template <typename G0, typename G1>
ProductGroup<G0, G1>::ProductGroup(const G0& g0, const G1& g1)
    : g0_{g0}, g1_{g1}
{
}

template <typename G0, typename G1>
const ProductGroup<G0, G1>
ProductGroup<G0, G1>::identity{G0::identity, G1::identity};

template <typename G0, typename G1>
ProductGroup<G0, G1>
ProductGroup<G0, G1>::inverse() const {
    return {g0_.inverse(), g1_.inverse()};
}

template <typename G0, typename G1>
ProductGroup<G0, G1>
ProductGroup<G0, G1>::operator * (
        const ProductGroup<G0, G1>& other) const
{
    return {g0_*other.g0_, g1_*other.g1_};
}

} // apollonian

#endif // GROUPS_HPP
