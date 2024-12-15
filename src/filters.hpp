/* SPDX-License-Identifier: GPL-3.0-only
 *
 * Copyright 2024 Darsh Ranjan.
 *
 * This file is part of super-apollonian-cpp.
 */

#ifndef FILTERS_HPP
#define FILTERS_HPP

#include <memory>

#include "color.hpp"
#include "image_buffer.hpp"

namespace apollonian {

class gaussian_kernel;

class unsharp_mask {
public:
    unsharp_mask(double radius, double amount);
    ~unsharp_mask();

    int padding() const;

    template <typename Pixel>
    image_buffer<Pixel> apply(const image_buffer<Pixel>& data) const;

private:
    std::unique_ptr<gaussian_kernel> blur_kernel_;
    double amount_;
};

template <>
image_buffer<double> unsharp_mask::apply<double>(
    const image_buffer<double>& data) const;

template <>
image_buffer<rgb_color> unsharp_mask::apply<rgb_color>(
    const image_buffer<rgb_color>& data) const;

} // apollonian

#endif // FILTERS_HPP
