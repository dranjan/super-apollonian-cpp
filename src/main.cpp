/* SPDX-License-Identifier: GPL-3.0-only
 *
 * Copyright 2024 Darsh Ranjan.
 *
 * This file is part of super-apollonian-cpp.
 */

#include <cmath>
#include <iostream>
#include <string>

#include "filters.hpp"
#include "io.hpp"
#include "visitor.hpp"

using namespace apollonian;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0]
                  << " ${output}.png"
                  << std::endl;
        return 2;
    }

    std::string filename(argv[1]);

    rgb_color c0(1.0, 0.0, 0.6);
    rgb_color c1(0.8, 0.0, 1.0);
    rgb_color c2(0.0, 0.6, 1.0);
    rgb_color c3(1.0, 0.6, 0.0);

    /* For convenience and testing, all post-filtering can be enabled or
     * disabled via this one boolean.
     */
    bool use_filters = true;

    unsharp_mask filter(5.0, 1.0);

    int padding = use_filters? filter.padding() : 0;

    /* These values can be increased to reduce computation for quicker
     * testing. Set both to 1 for the full rendering.
     */
    int scale_down = 1;           /* Increase to make a smaller image. */
    double threshold_factor = 1;  /* Increase to use fewer circles. */

    size_t w = 3840 / scale_down + 2*padding;
    size_t h = 2160 / scale_down + 2*padding;
    double res = 1000 / scale_down;
    rgb_color bgcolor = rgb_color::black;
    renderer renderer_(w, h, dcomplex(-2.4, -2.0), res);
    renderer_.fill(bgcolor);

    double f = -(2 + std::sqrt(3.0));
    dcomplex z = 0.6 + 0.8i;
    dcomplex a{f};
    dcomplex b{f*z};
    dcomplex c{f*z*z};

    rendering_visitor visitor{std::move(renderer_), threshold_factor/res,
                              {c0, c1, c2, c3}};

    int num_threads = std::thread::hardware_concurrency();
    int cell_size = 256;
    rendering_grid grid(num_threads, a, b, c, cell_size, cell_size, visitor);
    grid.run();

    if (use_filters) {
        std::cout << "applying post-processing filters..." << std::endl;
        auto image = filter.apply(visitor.buffer());
        std::cout << "done." << std::endl;
        save_image(image, filename);
    } else {
        save_image(visitor.buffer(), filename);
    }

    return 0;
}
