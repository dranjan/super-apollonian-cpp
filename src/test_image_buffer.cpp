#include <iostream>

#include "image_buffer.hpp"
#include "graphics.hpp"

using namespace apollonian;

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0]
                  << " ${output}.png"
                  << std::endl;
        return 2;
    }

    std::string filename(argv[1]);

    size_t w = 2160;
    size_t h = 2160;

    ImageBuffer<RGBColor> image(h, w);
    image.fill(RGBColor::white);
    draw_circle(image, 1080, 1080, 720,
                RGBColor::black, RGBColor::white);
    image.save(filename);

    return 0;
}
