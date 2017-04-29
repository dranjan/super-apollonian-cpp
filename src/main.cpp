#include <cmath>
#include <cassert>
#include <iostream>
#include <string>

#include "riemann_sphere.hpp"
#include "circle.hpp"
#include "apollonian.hpp"
#include "render.hpp"

using namespace apollonian;

class RenderingVisitor {
public:
    RenderingVisitor(CairoRenderer& renderer,
                     const RGBColor (*colors_)[4],
                     double r0, double threshold);

    bool operator () (const ApollonianState<int>& s);
    bool visit_node_a(const ApollonianState<int>& s);
    bool visit_node_b(const ApollonianState<int>& s);

    RGBColor get_color(const Circle& c, unsigned int index) const;

    void report() const;
    int transform(int data, NodeType type,
                  const ApollonianTransformation& t) const;

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
RenderingVisitor::operator () (const ApollonianState<int>& s) {
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
RenderingVisitor::visit_node_a(const ApollonianState<int>& s) {
    return s.size() >= threshold_;
}

bool
RenderingVisitor::visit_node_b(const ApollonianState<int>& s) {
    Circle c = s;
    renderer_.render_circle(c, get_color(c, s.t_.g1_.g_.v_[3]));
    ++count_;

    return s.data_ < 1 && s.size() >= threshold_;
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

int RenderingVisitor::transform(
        int data, NodeType type,
        const ApollonianTransformation& t) const
{
    (void)t;
    return data + (type == NodeType::B);
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
    generate_apollonian_gasket(a, b, c, 0, 1, visitor);

    visitor.report();
    renderer.save(filename);

    return 0;
}
