#include "riemann_sphere.hpp"
#include "circle.hpp"
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

    CairoRenderer renderer(2048, 2048,
                           Complex(0.0), 256,
                           RGBColor(1.0, 1.0, 1.0));
    renderer.render_circle(Circle::unit_circle,
                           RGBColor(0.0, 0.0, 0.0));
    renderer.save(filename);

    return 0;
}
