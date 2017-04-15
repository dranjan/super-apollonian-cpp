#ifndef RENDER_HPP
#define RENDER_HPP

#include <cmath>

#include <cairomm/context.h>
#include <cairomm/surface.h>

#include "riemann_sphere.hpp"
#include "color.hpp"
#include "circle.hpp"

namespace apollonian {

class CairoRenderer {
public:
    CairoRenderer(size_t w, size_t h, const Complex& center, double res,
                  const RGBColor& color);

public:
    void render_circle(const Circle& circle, const RGBColor& color);
    void save(const std::string& filename);

public:
    double xmin_;
    double xmax_;
    double ymin_;
    double ymax_;

    Cairo::RefPtr<Cairo::ImageSurface> surface_;
    Cairo::RefPtr<Cairo::Context> ctx_;
};

inline void
CairoRenderer::render_circle(const Circle& circle, const RGBColor& color)
{
    // TODO: handle lines as a degenerate case

    Complex center = circle.center();
    double radius = circle.radius();

    if (radius < 0) {
        radius = -radius;
        ctx_->rectangle(xmin_, ymin_, xmax_-xmin_, ymax_-ymin_);
    }
    ctx_->arc(center.real(), center.imag(), radius, 0, 2*M_PI);
    ctx_->set_source_rgb(color.r_, color.g_, color.b_);
    ctx_->fill();
}

} // apollonian

#endif // RENDER_HPP
