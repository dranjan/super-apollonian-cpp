#include <cmath>
#include <cassert>
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

        void set_fg();

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
                     double threshold);

    /* Callbacks */
    bool visit_node(const State& s);
    ExtraData get_data(const State& parent, NodeType type,
                       TransformationId id,
                       const ApollonianTransformation& t) const;

    void render(const PComplex& a, const PComplex& b, const PComplex& c);

    void report() const;

public:
    static RGBColor compute_fgcolor(const ExtraData& data);

protected:
    bool visit_node_a(const State& s);
    bool visit_node_b(const State& s);

private:
    Renderer& renderer_;
    double threshold_;
    int count_;
};

RenderingVisitor::ColorData
RenderingVisitor::ColorData::operator | (
        const RenderingVisitor::ColorData& other) const
{
    return level_ >= other.level_? *this : other;
}

RenderingVisitor::RenderingVisitor(Renderer& renderer,
                                   double threshold)
    : renderer_{renderer}, threshold_{threshold}, count_{0}
{
}

inline double get_component(double f) {
    return f/(1 + f);
}

inline void
RenderingVisitor::ExtraData::set_fg() {
    double w = c_[3]/3;
    self_fg_.color_ = RGBColor(get_component(c_[0] + w),
                               get_component(c_[1] + w),
                               get_component(c_[2] + w));
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
        data.set_fg();
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

    data0.set_fg();
    data1.set_fg();

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

    size_t w = 3840;
    size_t h = 2160;
    double res = 270;
    RGBColor bgcolor = RGBColor::black;
    Renderer renderer(w, h, Complex(-2.4, -2.0), res, bgcolor);

    double f = -(2 + std::sqrt(3.0));
    Complex z = 0.6 + 0.8i;
    Complex a{f};
    Complex b{f*z};
    Complex c{f*z*z};

    RenderingVisitor visitor{renderer, 1.0/res};
    visitor.render(a, b, c);
    visitor.report();
    renderer.save(filename);

    return 0;
}
