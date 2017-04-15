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

    Complex a(1.0);
    Complex b(std::exp(2*M_PI/3*i));
    Complex c(std::exp(-2*M_PI/3*i));

    ApollonianGenerators A
        = ApollonianGenerators::create(a, b, c);

    RGBColor colors[4] = {
        RGBColor(0xa0fa0f),
        RGBColor(0x0fa0fa),
        RGBColor(0xfa0fa0),
        RGBColor(0x111111),
    };

    double r0 = A.m0_.m_.inverse()(A.c_).radius();
    auto callback = [&r0, &colors, &A, &renderer] (const Circle& c,
                                                   const size_t* q)
    {
        double r1 = c.radius();
        if (r1 < 0 || r1 > r0) r1 = r0;
        double f = 1.0/(1.0 - 0.5*std::log(r1/r0));
        renderer.render_circle(c, colors[q[3]].blend(RGBColor::white, 1 - f));
    };

    A.iterate(1.0/res, callback);

    renderer.save(filename);

    return 0;
}
