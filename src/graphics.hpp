/* SPDX-License-Identifier: GPL-3.0-only
 *
 * Copyright 2024 Darsh Ranjan.
 *
 * This file is part of super-apollonian-cpp.
 */

/* This module implements some relatively low-level graphics primitives,
 * namely drawing the various types of generalized circle into an
 * image_buffer.
 *
 * All coordinates in the interface are in image pixels, unless noted
 * otherwise.
 *
 * This module takes advantage of the special nature of the Apollonian
 * fractal to produce highly accurate results with pretty low cost. The
 * key property is that if two generalized disks intersect at more than
 * a single point, then one must be contained in the other. Thus, to
 * draw a circle of a specific color `new_color`, if we know the color
 * `old_color` of the immediate parent circle, we can exactly remove the
 * parent's color from every affected pixel and perfectly render the new
 * circle as if it were being drawn on a blank image. This enables
 * effectively pixel-perfect rendering of the full set of circles, even
 * at the boundaries, and even at points where many circles are tangent.
 * Technically that's only true in exact arithmetic, but it does seem to
 * work extremely well in practice.
 */
#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "color.hpp"
#include "image_buffer.hpp"

namespace apollonian {

/* Draw the circle with radius r centered at (xc, yc). */
void
draw_circle(image_buffer<rgb_color>& image,
            double xc, double yc, double r,
            const rgb_color& new_color,
            const rgb_color& old_color);

/* Draw the complement of the circle with radius r centered at (xc, yc).
 */
void
draw_circle_complement(image_buffer<rgb_color>& image,
                       double xc, double yc, double r,
                       const rgb_color& new_color,
                       const rgb_color& old_color);

/* Draw the half-plane a*x + b*y + c <= 0. */
void
draw_half_plane(image_buffer<rgb_color>& image,
                double a, double b, double c,
                const rgb_color& new_color,
                const rgb_color& old_color);

} // apollonian

#endif // GRAPHICS_HPP
