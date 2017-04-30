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
        IntersectionType intersection_type;
        int level;
    };

    using State = ApollonianState<ExtraData>;

public:
    RenderingVisitor(CairoRenderer& renderer,
                     const RGBColor (*colors_)[4],
                     double r0, double threshold);

    bool visit_node(State& s);

    void report() const;

protected:
    bool visit_node_a(const State& s);
    bool visit_node_b(const State& s);
    void transform_data(State& s) const;

    RGBColor get_color(const Circle& c, unsigned int index) const;

private:
    CairoRenderer& renderer_;
    const RGBColor (*colors_)[4];
    double r0_;
    double threshold_;
    int count_;
};

RenderingVisitor::RenderingVisitor(CairoRenderer& renderer,
                                   const RGBColor (*colors)[4],
                                   double r0, double threshold)
    : renderer_{renderer}, colors_{colors}, r0_{r0},
      threshold_{threshold}, count_{0}
{
}

bool
RenderingVisitor::visit_node(State& s) {
    transform_data(s);
    if (s.data_.intersection_type == IntersectionType::Outside) {
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
};

bool
RenderingVisitor::visit_node_a(const State& s) {
    return s.size() >= threshold_;
}

bool
RenderingVisitor::visit_node_b(const State& s) {
    Circle c = s;
    renderer_.render_circle(c, get_color(c, s.t_.g1_.g_.v_[3]));
    ++count_;

    return s.data_.level < 1 && s.size() >= threshold_;
}

RGBColor
RenderingVisitor::get_color(const Circle& c, unsigned int index) const {
    double r1 = c.radius();
    if (r1 < 0 || r1 > r0_) {
        r1 = r0_;
    }

    double f = 1.0/(1.0 - 0.5*std::log(r1/r0_));
    return (*colors_)[index].blend(RGBColor::white, 1 - f);
}

void
RenderingVisitor::transform_data(State& s) const {
    s.data_.level += (s.type_ == NodeType::B);

    if (s.data_.intersection_type == IntersectionType::Intersects) {
        s.data_.intersection_type =
            renderer_.bbox_.intersects_circle(s);
    }
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

    size_t w = 2160;
    size_t h = 2160;
    double res = 270;
    CairoRenderer renderer(w, h, Complex(0.0), res,
                           RGBColor::black);

    RGBColor colors[4] = {
        RGBColor::decode24(0xa0fa0f),
        RGBColor::decode24(0x0fa0fa),
        RGBColor::decode24(0xfa0fa0),
        RGBColor::decode24(0x111111),
    };

    double f = -(2 + std::sqrt(3.0));
    Complex a{f};
    Complex b{f*std::exp(2i*(M_PI/3))};
    Complex c{f*std::exp(-2i*(M_PI/3))};

    double r0 = std::sqrt(3);

    RenderingVisitor visitor{renderer, &colors, r0, 1.0/res};
    RenderingVisitor::ExtraData data0{IntersectionType::Intersects, -1};
    RenderingVisitor::ExtraData data1{IntersectionType::Intersects, 0};
    generate_apollonian_gasket(a, b, c, data0, data1, visitor);

    visitor.report();
    renderer.save(filename);

    return 0;
}
