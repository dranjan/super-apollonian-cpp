/* SPDX-License-Identifier: GPL-3.0-only
 *
 * Copyright 2024 Darsh Ranjan.
 *
 * This file is part of super-apollonian-cpp.
 */

#ifndef VISITOR_HPP
#define VISITOR_HPP

#include <mutex>

#include "riemann_sphere.hpp"
#include "apollonian.hpp"
#include "render.hpp"

namespace apollonian {

/* Visitor object for generate_apollonian_gasket.
 */
class rendering_visitor {
public:
    struct color_data {
    public:
        rgb_color color_;
        int level_;

        color_data operator | (const color_data& other) const;
    };

    struct extra_data {
    public:
        extra_data() = default;
        extra_data(const extra_data&) = default;

    public:
        intersection_type intersection_type_;
        std::array<double, 4> c_;
        rgb_color bg_;
        color_data self_fg_;
        std::array<color_data, 3> point_bg_;
    };

    using state = apollonian_state<extra_data>;

public:
    rendering_visitor(renderer&& renderer_,
                      double threshold,
                      const std::array<std::array<double, 4>, 3>& color_table);
    rendering_visitor(renderer&& renderer_,
                      double threshold,
                      const std::array<rgb_color, 4>& colors);

    rendering_visitor window(int col0, int row0, int cols, int rows) const;

    /* Callbacks */
    bool visit_node(const state& s);
    extra_data get_data(const state& parent, node_type type,
                        canonical::transformation_id id,
                        const apollonian_transformation& t) const;

    void render(const pcomplex& a, const pcomplex& b, const pcomplex& c);
    void render_window(const pcomplex& a, const pcomplex& b, const pcomplex& c,
                       int col0, int row0, int cols, int rows,
                       std::mutex& mutex);

    void report() const;

    int cols() const;
    int rows() const;

    const image_buffer<rgb_color>& buffer() const {
        return renderer_.image_;
    }

protected:
    bool visit_node_a(const state& s);
    bool visit_node_b(const state& s);

    void set_fg(extra_data& extra) const;

private:
    renderer renderer_;
    double threshold_;
    int count_;

    /* indexed by [rgb_index][data_index] */
    std::array<std::array<double, 4>, 3> color_table_;
};

/* Top-level logic: multithreaded rendering implementation.
 * Wraps a rendering_visitor object with logic to subdivide the image
 * into subcells and render multiple cells in parallel.
 */
class rendering_grid {
public:
    rendering_grid(
        int num_threads,     /* Number of cells to render in parallel. */
        const pcomplex& z0,  /* z0, z1, z2: generate_apollonian_gasket args. */
        const pcomplex& z1,
        const pcomplex& z2,
        int cols, int rows,  /* Cell dimensions. */
        rendering_visitor& visitor);

    void run();

private:
    bool next_cell(int& col0, int& row0);
    void do_work();

private:
    /* Constants */
    pcomplex z0_;
    pcomplex z1_;
    pcomplex z2_;
    int cols_;
    int rows_;
    int num_threads_;

    /* State */
    int col0_;
    int row0_;

    std::mutex dispatch_mutex_;

    rendering_visitor* visitor_;
    std::mutex render_mutex_;
};

} // apollonian

#endif // VISITOR_HPP
