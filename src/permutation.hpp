/* SPDX-License-Identifier: GPL-3.0-only
 *
 * Copyright 2024 Darsh Ranjan.
 *
 * This file is part of super-apollonian-cpp.
 */

#ifndef PERMUTATION_HPP
#define PERMUTATION_HPP

#include <array>

namespace apollonian {

template <unsigned int N>
class permutation {
public:
    permutation();
    permutation(const permutation&) = default;

    template <typename... T>
    permutation(T... args);

    permutation<N> inverse() const;
    permutation<N> operator * (const permutation<N>& other) const;

    template <typename RandomAccessIteratorIn,
              typename RandomAccessIteratorOut>
    void operator () (const RandomAccessIteratorIn in_ptr,
                      RandomAccessIteratorOut out_ptr) const;

    static permutation<N> transposition(unsigned int i,
                                        unsigned int j);

    static const permutation<N> identity;

public:
    std::array<unsigned int, N> v_;
};

template <unsigned int N>
permutation<N>::permutation() {
    for (unsigned int k = 0; k < N; ++k) {
        v_[k] = k;
    }
}

template <unsigned int N>
template <typename... T>
permutation<N>::permutation(T... args)
    : v_{(unsigned int)(args)...}
{
}

template <unsigned int N>
permutation<N>
permutation<N>::inverse() const {
    permutation<N> p;
    for (unsigned int k = 0; k < N; ++k) {
        p.v_[v_[k]] = k;
    }
    return p;
}

template <unsigned int N>
permutation<N>
permutation<N>::operator * (const permutation<N>& other) const
{
    permutation<N> p;
    (*this)(other.v_.cbegin(), p.v_.begin());
    return p;
}

template <unsigned int N>
permutation<N>
permutation<N>::transposition(unsigned int i, unsigned int j) {
    permutation<N> p;
    p.v_[i] = j;
    p.v_[j] = i;
    return p;
}

template <unsigned int N>
template <typename RandomAccessIteratorIn,
          typename RandomAccessIteratorOut>
void
permutation<N>::operator () (
        const RandomAccessIteratorIn in_ptr,
        RandomAccessIteratorOut out_ptr) const
{
    for (unsigned int k = 0; k < N; ++k) {
        *(out_ptr + k) = *(in_ptr + v_[k]);
    }
}

template <unsigned int N>
const permutation<N>
permutation<N>::identity;

} // apollonian

#endif // PERMUTATION_HPP
