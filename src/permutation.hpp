#ifndef PERMUTATION_HPP
#define PERMUTATION_HPP

#include <cstdlib>

namespace apollonian {

template <size_t N>
class Permutation {
public:
    Permutation();

    template <typename... T>
    Permutation(T... args);

    Permutation<N> inverse() const;
    Permutation<N> operator * (const Permutation<N>& other) const;

    template <typename RandomAccessIteratorIn,
              typename RandomAccessIteratorOut>
    void operator () (const RandomAccessIteratorIn in_ptr,
                      RandomAccessIteratorOut out_ptr) const;

    static Permutation<N> transposition(size_t i, size_t j);

    static const Permutation<N> identity;

public:
    size_t v_[N];
};

template <size_t N>
Permutation<N>::Permutation() {
    for (size_t k = 0; k < N; ++k) {
        v_[k] = k;
    }
}

template <size_t N>
template <typename... T>
Permutation<N>::Permutation(T... args)
    : v_{args...}
{
}

template <size_t N>
Permutation<N>
Permutation<N>::inverse() const {
    Permutation<N> p;
    for (size_t k = 0; k < N; ++k) {
        p.v_[v_[k]] = k;
    }
    return p;
}

template <size_t N>
Permutation<N>
Permutation<N>::operator * (const Permutation<N>& other) const
{
    Permutation<N> p;
    (*this)(other.v_, p.v_);
    return p;
}

template <size_t N>
Permutation<N>
Permutation<N>::transposition(size_t i, size_t j) {
    Permutation<N> p;
    p.v_[i] = j;
    p.v_[j] = i;
    return p;
}

template <size_t N>
template <typename RandomAccessIteratorIn,
          typename RandomAccessIteratorOut>
void
Permutation<N>::operator () (
        const RandomAccessIteratorIn in_ptr,
        RandomAccessIteratorOut out_ptr) const
{
    for (size_t k = 0; k < N; ++k) {
        *(out_ptr + k) = *(in_ptr + v_[k]);
    }
}

template <size_t N>
const Permutation<N>
Permutation<N>::identity;

} // apollonian

#endif // PERMUTATION_HPP
