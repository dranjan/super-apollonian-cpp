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

class RenderingVisitor {
public:
    struct ExtraData {
    public:
        ExtraData() = default;
        ExtraData(IntersectionType intersection_type,
                  const std::array<double, 4>& c,
                  const RGBColor& bg);

        void set_fg();

    public:
        IntersectionType intersection_type_;
        std::array<double, 4> c_;
        RGBColor bg_;
        RGBColor fg_;
    };

    using State = ApollonianState<ExtraData>;

public:
    RenderingVisitor(Renderer& renderer,
                     const RGBColor (*colors_)[4],
                     double r0, double threshold);

    bool visit_node(const State& s);
    ExtraData get_data(const State& parent, NodeType type,
                       const ApollonianTransformation& t) const;

    void report() const;

public:
    static RGBColor compute_fgcolor(const ExtraData& data);

protected:
    bool visit_node_a(const State& s);
    bool visit_node_b(const State& s);

private:
    Renderer& renderer_;
    const RGBColor (*colors_)[4];
    double r0_;
    double threshold_;
    int count_;
};

RenderingVisitor::ExtraData::ExtraData(IntersectionType intersection_type,
                                       const std::array<double, 4>& c,
                                       const RGBColor& bg)
    : intersection_type_{intersection_type}, c_{c}, bg_{bg}
{
    set_fg();
}

RenderingVisitor::RenderingVisitor(Renderer& renderer,
                                   const RGBColor (*colors)[4],
                                   double r0, double threshold)
    : renderer_{renderer}, colors_{colors}, r0_{r0},
      threshold_{threshold}, count_{0}
{
}

inline double get_component(double f) {
    return f/(1 + f);
}

inline void
RenderingVisitor::ExtraData::set_fg() {
    double w = c_[3]/3;
    fg_ = RGBColor(get_component(c_[0] + w),
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
    renderer_.render_circle(c, s.data_.fg_, s.data_.bg_);
    ++count_;

    return s.size() >= threshold_;
}

RenderingVisitor::ExtraData
RenderingVisitor::get_data(const State& parent, NodeType type,
                           const ApollonianTransformation& t) const
{
    ExtraData data = parent.data_;
    Circle c = t.g0_(canonical::c);

    if (data.intersection_type_ == IntersectionType::Intersects) {
        data.intersection_type_ =
            renderer_.bbox_.intersects_circle(c);
    }
    if (type == NodeType::B &&
        data.intersection_type_ != IntersectionType::Outside)
    {
        double r = std::abs(c.radius());
        double f = 0.25 * std::pow(1/(1/r + r)*4, 0.6);
        data.c_[t.g1_.g_.v_[3]] += f;

        data.bg_ = data.fg_;
        data.set_fg();
    }

    return data;
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

    RGBColor colors[4] = {
        RGBColor::decode24(0xa0fa0f),
        RGBColor::decode24(0x0fa0fa),
        RGBColor::decode24(0xfa0fa0),
        RGBColor::decode24(0x111111),
    };

    double f = -(2 + std::sqrt(3.0));
    Complex z = 0.6 + 0.8i;
    Complex a{f};
    Complex b{f*z};
    Complex c{f*z*z};

    double r0 = std::sqrt(3);

    RenderingVisitor visitor{renderer, &colors, r0, 1.0/res};
    RenderingVisitor::ExtraData data0{IntersectionType::Intersects,
                                      {0, 0, 0, 0.1}, bgcolor};
    RenderingVisitor::ExtraData data1{IntersectionType::Intersects,
                                      {0, 0, 0, 0}, bgcolor};
    generate_apollonian_gasket(a, b, c, data0, data1, visitor);

    visitor.report();
    renderer.save(filename);

    return 0;
}
