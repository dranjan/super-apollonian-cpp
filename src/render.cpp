#include "render.hpp"

namespace apollonian {

Renderer::Renderer(
        int w, int h, const Complex& center, double pixel_res,
        const RGBColor& color)
    : bbox_{make_box(center, w/pixel_res, h/pixel_res)},
      image_{h, w}, center_{center}, res_{pixel_res}
{
    image_.fill(color);
}

template <typename T>
inline T clamp(const T& value, const T& min, const T& max) {
    return std::min(max, std::max(min, value));
}
inline uint8_t
get_component(double value) {
    return clamp(value, 0.0, 1.0)*0xff;
}

inline unsigned char*
write_pixel(const RGBColor& pixel, unsigned char* p) {
    *((uint32_t*)p) = (get_component(pixel.b_) << 0) +
                      (get_component(pixel.g_) << 8) +
                      (get_component(pixel.r_) << 16) +
                      (0xff << 24);
    return p + 4;
}

void Renderer::save(const std::string& filename) const {
    int rows = image_.rows();
    int cols = image_.cols();

    Cairo::RefPtr<Cairo::ImageSurface> surface =
        Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, cols, rows);

    unsigned char* data = surface->get_data();
    int stride = surface->get_stride();
    for (int row = 0; row < rows; ++row) {
        unsigned char* p = data + row*stride;
        for (int col = 0; col < cols; ++col) {
            p = write_pixel(image_(row, col), p);
        }
    }
    surface->write_to_png(filename);
}

} // apollonian
