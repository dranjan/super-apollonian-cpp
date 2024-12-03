/* SPDX-License-Identifier: GPL-3.0-only
 *
 * Copyright 2024 Darsh Ranjan.
 *
 * This file is part of super-apollonian-cpp.
 */

#ifndef RENDER_HPP
#define RENDER_HPP

#include <cmath>

#include "riemann_sphere.hpp"
#include "color.hpp"
#include "circle.hpp"
#include "box.hpp"
#include "image_buffer.hpp"
#include "graphics.hpp"

namespace apollonian {

class renderer {
public:
    renderer(double x0, double y0, int w, int h, double res);
    renderer(int w, int h, const dcomplex& center, double res);

public:
    void render_circle(const circle& circle, const rgb_color& new_color,
                       const rgb_color& old_color);
    void fill(const rgb_color& color);
    void save(const std::string& filename) const;
    void map(const dcomplex& z, double& col, double& row) const;
    dcomplex unmap(double col, double row) const;
    intersection_type intersects_circle(const circle& c) const;

    renderer window(int col0, int row0, int cols, int rows) const;
    void set_window(int col0, int row0, const renderer& window);

public:
    double x0_;
    double y0_;
    box bbox_;
    image_buffer<rgb_color> image_;
    double res_;
};

inline intersection_type
renderer::intersects_circle(const circle& c) const {
    return bbox_.intersects_circle(c);
}

inline void
renderer::render_circle(const circle& circle, const rgb_color& new_color,
                        const rgb_color& old_color)
{
    if (circle.v00_ == 0) {
        double a = 2*circle.v01_.real()/res_;
        double b = 2*circle.v01_.imag()/res_;
        double c = circle.v11_ + 2*(circle.v01_.real()*x0_
                                  + circle.v01_.imag()*y0_);
        draw_half_plane(image_, a, b, c, new_color, old_color);
    } else {
        double xc;
        double yc;
        map(circle.center(), xc, yc);
        double r = circle.radius()*res_;
        if (r < 0) {
            draw_circle_complement(image_, xc, yc, -r, new_color, old_color);
        } else {
            draw_circle(image_, xc, yc, r, new_color, old_color);
        }
    }
}

inline void
renderer::fill(const rgb_color& color) {
    image_.fill(color);
}

inline void
renderer::map(const dcomplex& z, double& col, double& row) const {
    col = res_*(z.real() - x0_);
    row = res_*(z.imag() - y0_);
}

inline dcomplex
renderer::unmap(double col, double row) const {
    return {x0_ + col/res_,
            y0_ + row/res_};
}

} // apollonian

#endif // RENDER_HPP
