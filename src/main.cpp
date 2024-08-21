#include <cmath>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <string>

#include "riemann_sphere.hpp"
#include "circle.hpp"
#include "apollonian.hpp"
#include "render.hpp"
#include "box.hpp"

using namespace apollonian;
using canonical::transformation_id;

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
    rendering_visitor(renderer& renderer_,
                      double threshold,
                      const std::array<rgb_color, 4>& colors);

    /* Callbacks */
    bool visit_node(const state& s);
    extra_data get_data(const state& parent, node_type type,
                        transformation_id id,
                        const apollonian_transformation& t) const;

    void render(const pcomplex& a, const pcomplex& b, const pcomplex& c);

    void report() const;

protected:
    bool visit_node_a(const state& s);
    bool visit_node_b(const state& s);

    void set_fg(extra_data& extra) const;

private:
    renderer& renderer_;
    double threshold_;
    int count_;

    /* indexed by [rgb_index][data_index] */
    std::array<std::array<double, 4>, 3> color_table_;
};

rendering_visitor::color_data
rendering_visitor::color_data::operator | (
        const rendering_visitor::color_data& other) const
{
    return level_ >= other.level_? *this : other;
}

rendering_visitor::rendering_visitor(
    renderer& renderer_,
    double threshold,
    const std::array<rgb_color, 4>& colors)
    : renderer_{renderer_}, threshold_{threshold}, count_{0}
{
    for (int k = 0; k < 4; ++k) {
        color_table_[0][k] = double(colors[k].r_)/0x7fffffff;
        color_table_[1][k] = double(colors[k].g_)/0x7fffffff;
        color_table_[2][k] = double(colors[k].b_)/0x7fffffff;
    }
}

inline void
rendering_visitor::set_fg(extra_data& data) const {
    double rgb[3] = {0.0, 0.0, 0.0};
    for (int k = 0; k < 3; ++k) {
      for (int j = 0; j < 4; ++j) {
        rgb[k] += color_table_[k][j]*data.c_[j] / 2;
      }
    }
    /* This may look a bit arbitrary, but here's an explanation.
     * - Since the color computation can potentially give unbounded
     *   results, first we scale everything back to [0, 1).
     * - We apply a secondary scaling to bring the result closer to
     *   white at the brightest points, which makes the result look
     *   a bit nicer.
     */
    double m = std::max({rgb[0], rgb[1], rgb[2]});
    double mm = m*m;
    double g = 1/(1 + m);
    double q = (mm*mm)/16;
    double f = 1/(1 + q/(1 + q));
    for (int k = 0; k < 3; ++k) {
        rgb[k] *= g;
        rgb[k] = 1 - f + f*rgb[k];
    }
    data.self_fg_.color_ = rgb_color(rgb[0], rgb[1], rgb[2]);
}

bool
rendering_visitor::visit_node(const state& s) {
    if (s.data_.intersection_type_ == intersection_type::outside) {
        return false;
    }

    switch (s.type_) {
    case node_type::A:
        return visit_node_a(s);
    case node_type::B:
        return visit_node_b(s);
    default:
        assert(false);
    }

    return false;
}

bool
rendering_visitor::visit_node_a(const state& s) {
    return s.size() >= threshold_;
}

bool
rendering_visitor::visit_node_b(const state& s) {
    circle c = s;
    renderer_.render_circle(c, s.data_.self_fg_.color_, s.data_.bg_);
    ++count_;

    return s.size() >= threshold_;
}

rendering_visitor::extra_data
rendering_visitor::get_data(const state& parent, node_type type,
                            transformation_id id,
                            const apollonian_transformation& t) const
{
    extra_data data = parent.data_;
    circle c = t.g0_(canonical::c);

    switch (id) {
    case transformation_id::M0:
        break;
    case transformation_id::M1:
        break;
    case transformation_id::M2:
        break;
    case transformation_id::N0:
        break;
    case transformation_id::N1:
        break;
    case transformation_id::N2:
        break;
    case transformation_id::P:
        break;
    default:
        assert(false);
    }

    if (data.intersection_type_ == intersection_type::intersects) {
        data.intersection_type_ =
            renderer_.bbox_.intersects_circle(c);
    }
    if (type == node_type::B &&
        data.intersection_type_ != intersection_type::outside)
    {
        ++data.self_fg_.level_;

        double r = std::abs(c.radius());
        double f = 0.25 * std::pow(1/(1/r + r)*4, 0.6);
        data.c_[t.g1_.g_.v_[3]] += f;

        data.bg_ = data.self_fg_.color_;
        set_fg(data);
    }

    return data;
}

void
rendering_visitor::render(const pcomplex& a,
                          const pcomplex& b,
                          const pcomplex& c)
{
    extra_data data0;
    data0.intersection_type_ = intersection_type::intersects;
    data0.c_[0] = 0;
    data0.c_[1] = 0;
    data0.c_[2] = 0;
    data0.c_[3] = 0.1;
    data0.bg_ = rgb_color::black;
    data0.self_fg_.level_ = 1;

    extra_data data1;
    data1.intersection_type_ = intersection_type::intersects;
    data1.c_[0] = 0;
    data1.c_[1] = 0;
    data1.c_[2] = 0;
    data1.c_[3] = 0;
    data1.bg_ = rgb_color::black;
    data1.self_fg_.level_ = 0;

    set_fg(data0);
    set_fg(data1);

    data0.point_bg_[0] = data0.self_fg_ | data1.self_fg_;
    data0.point_bg_[1] = data0.self_fg_ | data1.self_fg_;
    data0.point_bg_[2] = data0.self_fg_ | data1.self_fg_;

    data1.point_bg_[0] = data0.self_fg_ | data1.self_fg_;
    data1.point_bg_[1] = data0.self_fg_ | data1.self_fg_;
    data1.point_bg_[2] = data0.self_fg_ | data1.self_fg_;

    generate_apollonian_gasket(a, b, c, data0, data1, *this);
}

void
rendering_visitor::report() const {
    std::cout << "Circles rendered: " << count_ << std::endl;
}

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

    /* These values can be increased to reduce computation for quicker
     * testing. Set both to 1 for the full rendering.
     */
    int scale_down = 1;           /* Increase to make a smaller image. */
    double threshold_factor = 1;  /* Increase to use fewer circles. */

    size_t w = 3840 / scale_down;
    size_t h = 2160 / scale_down;
    double res = 1000 / scale_down;
    rgb_color bgcolor = rgb_color::black;
    renderer renderer_(w, h, dcomplex(-2.4, -2.0), res, bgcolor);

    double f = -(2 + std::sqrt(3.0));
    dcomplex z = 0.6 + 0.8i;
    dcomplex a{f};
    dcomplex b{f*z};
    dcomplex c{f*z*z};

    rendering_visitor visitor{renderer_, threshold_factor/res,
                              {c0, c1, c2, c3}};
    visitor.render(a, b, c);
    visitor.report();
    renderer_.save(filename);

    return 0;
}
