#include <cmath>
#include <iostream>
#include <string>

#include "riemann_sphere.hpp"
#include "circle.hpp"
#include "apollonian.hpp"
#include "render.hpp"

using namespace apollonian;

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

    Complex a{1.0};
    Complex b{std::exp(2i*(M_PI/3))};
    Complex c{std::exp(-2i*(M_PI/3))};

    MobiusTransformation m
        = MobiusTransformation::cross_ratio(a, b, c).inverse();
    double r0 = m(ApollonianState::c0).radius();

    auto callback
        = [&r0, &colors, &renderer] (const ApollonianState& s)
    {
        Circle c = s;
        double r1 = c.radius();
        if (r1 < 0 || r1 > r0) r1 = r0;
        double f = 1.0/(1.0 - 0.5*std::log(r1/r0));
        RGBColor color = colors[s.p_.v_[3]].blend(RGBColor::white,
                                                  1 - f);
        renderer.render_circle(c, color);
    };

    generate_apollonian_gasket(a, b, c, 1.0/res, callback);

    renderer.save(filename);

    return 0;
}
