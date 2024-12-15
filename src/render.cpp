/* SPDX-License-Identifier: GPL-3.0-only
 *
 * Copyright 2024 Darsh Ranjan.
 *
 * This file is part of super-apollonian-cpp.
 */

#include "render.hpp"

#include <algorithm>

namespace apollonian {

renderer::renderer(double x0, double y0, int w, int h, double res)
    : x0_{x0}, y0_{y0}, image_{h, w}, res_{res}
{
    dcomplex z1 = unmap(image_.cols(), image_.rows());
    bbox_ = {x0_, z1.real(), y0_, z1.imag()};
}

renderer::renderer(
        int w, int h, const dcomplex& center, double res)
    : renderer{center.real() - 0.5*w/res, center.imag() - 0.5*h/res, w, h, res}
{
}

renderer renderer::window(int col0, int row0, int cols, int rows) const {
    if (col0 + cols > image_.cols()) cols = image_.cols() - col0;
    if (row0 + rows > image_.rows()) rows = image_.rows() - row0;
    dcomplex z0 = unmap(col0, row0);
    renderer window_renderer{z0.real(), z0.imag(), cols, rows, res_};
    for (int row = 0; row < rows; ++row) {
        const rgb_color* src = image_[row0 + row] + col0;
        rgb_color* dst = window_renderer.image_[row];
        std::copy(src, src + cols, dst);
    }
    return window_renderer;
}

void renderer::set_window(int col0, int row0, const renderer& window) {
    int rows = window.image_.rows();
    int cols = window.image_.cols();
    for (int row = 0; row < rows; ++row) {
        const rgb_color* src = window.image_[row];
        rgb_color* dst = image_[row0 + row] + col0;
        std::copy(src, src + cols, dst);
    }
}

} // apollonian
