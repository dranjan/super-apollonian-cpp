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
using canonical::TransformationId;

class RenderingVisitor {
public:
    struct ColorData {
    public:
        RGBColor color_;
        int level_;

        ColorData operator | (const ColorData& other) const;
    };

    struct ExtraData {
    public:
        ExtraData() = default;
        ExtraData(const ExtraData&) = default;

    public:
        IntersectionType intersection_type_;
        std::array<double, 4> c_;
        RGBColor bg_;
        ColorData self_fg_;
        std::array<ColorData, 3> point_bg_;
    };

    using State = ApollonianState<ExtraData>;

public:
    RenderingVisitor(Renderer& renderer,
                     double threshold,
                     const std::array<RGBColor, 4>& colors);

    /* Callbacks */
    bool visit_node(const State& s);
    ExtraData get_data(const State& parent, NodeType type,
                       TransformationId id,
                       const ApollonianTransformation& t) const;

    void render(const PComplex& a, const PComplex& b, const PComplex& c);

    void report() const;

protected:
    bool visit_node_a(const State& s);
    bool visit_node_b(const State& s);

    void set_fg(ExtraData& extra) const;

private:
    Renderer& renderer_;
    double threshold_;
    int count_;

    /* indexed by [rgb_index][data_index] */
    std::array<std::array<double, 4>, 3> color_table_;
};

RenderingVisitor::ColorData
RenderingVisitor::ColorData::operator | (
        const RenderingVisitor::ColorData& other) const
{
    return level_ >= other.level_? *this : other;
}

RenderingVisitor::RenderingVisitor(
    Renderer& renderer,
    double threshold,
    const std::array<RGBColor, 4>& colors)
    : renderer_{renderer}, threshold_{threshold}, count_{0}
{
    for (int k = 0; k < 4; ++k) {
        color_table_[0][k] = double(colors[k].r_)/0x7fffffff;
        color_table_[1][k] = double(colors[k].g_)/0x7fffffff;
        color_table_[2][k] = double(colors[k].b_)/0x7fffffff;
    }
}

inline void
RenderingVisitor::set_fg(ExtraData& data) const {
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
    data.self_fg_.color_ = RGBColor(rgb[0], rgb[1], rgb[2]);
}

bool
RenderingVisitor::visit_node(const State& s) {
    if (s.data_.intersection_type_ == IntersectionType::Outside) {
        return false;
    }

    switch (s.type_) {
    case NodeType::A:
        return visit_node_a(s);
    case NodeType::B:
        return visit_node_b(s);
    default:
        assert(false);
    }

    return false;
}

bool
RenderingVisitor::visit_node_a(const State& s) {
    return s.size() >= threshold_;
}

bool
RenderingVisitor::visit_node_b(const State& s) {
    Circle c = s;
    renderer_.render_circle(c, s.data_.self_fg_.color_, s.data_.bg_);
    ++count_;

    return s.size() >= threshold_;
}

RenderingVisitor::ExtraData
RenderingVisitor::get_data(const State& parent, NodeType type,
                           TransformationId id,
                           const ApollonianTransformation& t) const
{
    ExtraData data = parent.data_;
    Circle c = t.g0_(canonical::c);

    switch (id) {
    case TransformationId::M0:
        break;
    case TransformationId::M1:
        break;
    case TransformationId::M2:
        break;
    case TransformationId::N0:
        break;
    case TransformationId::N1:
        break;
    case TransformationId::N2:
        break;
    case TransformationId::P:
        break;
    default:
        assert(false);
    }

    if (data.intersection_type_ == IntersectionType::Intersects) {
        data.intersection_type_ =
            renderer_.bbox_.intersects_circle(c);
    }
    if (type == NodeType::B &&
        data.intersection_type_ != IntersectionType::Outside)
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
RenderingVisitor::render(const PComplex& a,
                         const PComplex& b,
                         const PComplex& c)
{
    ExtraData data0;
    data0.intersection_type_ = IntersectionType::Intersects;
    data0.c_[0] = 0;
    data0.c_[1] = 0;
    data0.c_[2] = 0;
    data0.c_[3] = 0.1;
    data0.bg_ = RGBColor::black;
    data0.self_fg_.level_ = 1;

    ExtraData data1;
    data1.intersection_type_ = IntersectionType::Intersects;
    data1.c_[0] = 0;
    data1.c_[1] = 0;
    data1.c_[2] = 0;
    data1.c_[3] = 0;
    data1.bg_ = RGBColor::black;
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
RenderingVisitor::report() const {
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

    RGBColor c0(1.0, 0.0, 0.6);
    RGBColor c1(0.8, 0.4, 0.0);
    RGBColor c2(0.0, 0.6, 1.0);
    RGBColor c3(0.3, 0.5, 0.3);

    /* These values can be increased to reduce computation for quicker
     * testing. Set both to 1 for the full rendering.
     */
    int scale_down = 1;           /* Increase to make a smaller image. */
    double threshold_factor = 1;  /* Increase to use fewer circles. */

    size_t w = 3840 / scale_down;
    size_t h = 2160 / scale_down;
    double res = 540 / scale_down;
    RGBColor bgcolor = RGBColor::black;
    Renderer renderer(w, h, Complex(-2.4, -2.0), res, bgcolor);

    double f = -(2 + std::sqrt(3.0));
    Complex z = 0.6 + 0.8i;
    Complex a{f};
    Complex b{f*z};
    Complex c{f*z*z};

    RenderingVisitor visitor{renderer, threshold_factor/res,
                             {c0, c1, c2, c3}};
    visitor.render(a, b, c);
    visitor.report();
    renderer.save(filename);

    return 0;
}
