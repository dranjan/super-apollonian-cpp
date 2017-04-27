#ifndef PERMUTATION_HPP
#define PERMUTATION_HPP

#include <array>

namespace apollonian {

template <unsigned int N>
class Permutation {
public:
    Permutation();
    Permutation(const Permutation&) = default;

    template <typename... T>
    Permutation(T... args);

    Permutation<N> inverse() const;
    Permutation<N> operator * (const Permutation<N>& other) const;

    template <typename RandomAccessIteratorIn,
              typename RandomAccessIteratorOut>
    void operator () (const RandomAccessIteratorIn in_ptr,
                      RandomAccessIteratorOut out_ptr) const;

    static Permutation<N> transposition(unsigned int i,
                                        unsigned int j);

    static const Permutation<N> identity;

public:
    std::array<unsigned int, N> v_;
};

template <unsigned int N>
Permutation<N>::Permutation() {
    for (unsigned int k = 0; k < N; ++k) {
        v_[k] = k;
    }
}

template <unsigned int N>
template <typename... T>
Permutation<N>::Permutation(T... args)
    : v_{(unsigned int)(args)...}
{
}

template <unsigned int N>
Permutation<N>
Permutation<N>::inverse() const {
    Permutation<N> p;
    for (unsigned int k = 0; k < N; ++k) {
        p.v_[v_[k]] = k;
    }
    return p;
}

template <unsigned int N>
Permutation<N>
Permutation<N>::operator * (const Permutation<N>& other) const
{
    Permutation<N> p;
    (*this)(other.v_.cbegin(), p.v_.begin());
    return p;
}

template <unsigned int N>
Permutation<N>
Permutation<N>::transposition(unsigned int i, unsigned int j) {
    Permutation<N> p;
    p.v_[i] = j;
    p.v_[j] = i;
    return p;
}

template <unsigned int N>
template <typename RandomAccessIteratorIn,
          typename RandomAccessIteratorOut>
void
Permutation<N>::operator () (
        const RandomAccessIteratorIn in_ptr,
        RandomAccessIteratorOut out_ptr) const
{
    for (unsigned int k = 0; k < N; ++k) {
        *(out_ptr + k) = *(in_ptr + v_[k]);
    }
}

template <unsigned int N>
const Permutation<N>
Permutation<N>::identity;

} // apollonian

#endif // PERMUTATION_HPP
