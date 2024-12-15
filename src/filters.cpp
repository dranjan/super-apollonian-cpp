/* SPDX-License-Identifier: GPL-3.0-only
 *
 * Copyright 2024 Darsh Ranjan.
 *
 * This file is part of super-apollonian-cpp.
 */

#include "filters.hpp"

#include <array>
#include <cmath>

namespace apollonian {

std::array<image_buffer<double>, 3>
get_channels(const image_buffer<rgb_color>& image) {
    int rows = image.rows();
    int cols = image.cols();
    std::array<image_buffer<double>, 3> channels = {{
        {rows, cols},
        {rows, cols},
        {rows, cols}
    }};
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            channels[0](row, col) = double(image(row, col).r_) / 0x7fffffff;
            channels[1](row, col) = double(image(row, col).g_) / 0x7fffffff;
            channels[2](row, col) = double(image(row, col).b_) / 0x7fffffff;
        }
    }
    return channels;
}

inline double normalize_channel(double x) {
    if (x < 0) x = 0;
    if (x > 1) x = 1;
    return x;
}

image_buffer<rgb_color>
get_image(const image_buffer<double>& r,
          const image_buffer<double>& g,
          const image_buffer<double>& b)
{
    int rows = r.rows();
    int cols = r.cols();
    image_buffer<rgb_color> image(rows, cols);
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            image(row, col) = rgb_color(
                normalize_channel(r(row, col)),
                normalize_channel(g(row, col)),
                normalize_channel(b(row, col)));
        }
    }
    return image;
}

class gaussian_kernel {
public:
    gaussian_kernel(double radius, int cutoff);

    template <typename Pixel>
    image_buffer<Pixel> apply_2d(const image_buffer<Pixel>& data) const;

    int order() const {
        return coeffs_.size();
    }

    int shift() const {
        return (coeffs_.size() - 1)/2;
    }

private:
    template <typename Pixel>
    image_buffer<Pixel> apply_x(const image_buffer<Pixel>& data) const;

    template <typename Pixel>
    image_buffer<Pixel> apply_y(const image_buffer<Pixel>& data) const;

private:
    std::vector<double> coeffs_;
};

gaussian_kernel::gaussian_kernel(double radius, int cutoff)
    : coeffs_(2*cutoff + 1)
{
    int n = coeffs_.size();
    int center = cutoff;
    double total = 0;
    for (int k = 0; k < n; ++k) {
        double x = (k - center)/radius;
        coeffs_[k] = std::exp(-x*x*0.5);
        total += coeffs_[k];
    }
    for (int k = 0; k < n; ++k) coeffs_[k] /= total;
}

template <typename Pixel>
image_buffer<Pixel>
gaussian_kernel::apply_x(const image_buffer<Pixel>& data) const {
    int n = order();
    image_buffer<Pixel> result(data.rows(), data.cols() - n + 1);
    int cols = result.cols();
    int rows = result.rows();
    for (int row = 0; row < rows; ++ row) {
        for (int col = 0; col < cols; ++col) {
            Pixel& p = result(row, col);
            p = Pixel(0);
            for (int k = 0; k < n; ++k) {
                p += data(row, col + k)*coeffs_[k];
            }
        }
    }
    return result;
}

template <typename Pixel>
image_buffer<Pixel>
gaussian_kernel::apply_y(const image_buffer<Pixel>& data) const {
    int n = order();
    image_buffer<Pixel> result(data.rows() - n + 1, data.cols());
    int cols = result.cols();
    int rows = result.rows();
    for (int row = 0; row < rows; ++ row) {
        for (int col = 0; col < cols; ++col) {
            Pixel& p = result(row, col);
            p = Pixel(0);
            for (int k = 0; k < n; ++k) {
                p += data(row + k, col)*coeffs_[k];
            }
        }
    }
    return result;
}

template <typename Pixel>
image_buffer<Pixel>
gaussian_kernel::apply_2d(const image_buffer<Pixel>& data) const {
    image_buffer<Pixel> result = apply_x(data);
    return apply_y(result);
}

unsharp_mask::unsharp_mask(double radius, double amount) : amount_(amount) {
    int cutoff = int(radius*4);
    blur_kernel_ = std::make_unique<gaussian_kernel>(radius, cutoff);
}

unsharp_mask::~unsharp_mask() {
}

int unsharp_mask::padding() const {
    return blur_kernel_->shift();
}

template <>
image_buffer<double>
unsharp_mask::template apply<double>(const image_buffer<double>& data) const {
    image_buffer<double> data_blurred = blur_kernel_->apply_2d(data);
    int rows = data_blurred.rows();
    int cols = data_blurred.cols();
    int shift = padding();
    image_buffer<double> result(rows, cols);
    for (int row = 0; row < rows; ++ row) {
        for (int col = 0; col < cols; ++col) {
            double p = data(row + shift, col + shift);
            double q = data_blurred(row, col);
            result(row, col) = p + (p - q)*amount_;
        }
    }
    return result;
}

template <>
image_buffer<rgb_color>
unsharp_mask::template apply<rgb_color>(
    const image_buffer<rgb_color>& data) const
{
    auto channels = get_channels(data);
    std::array<image_buffer<double>, 3> result_channels = {
        apply(channels[0]),
        apply(channels[1]),
        apply(channels[2])
    };
    return get_image(result_channels[0],
                     result_channels[1],
                     result_channels[2]);
}

} // apollonian
