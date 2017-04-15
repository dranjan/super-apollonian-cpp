#include "render.hpp"

namespace apollonian {

CairoRenderer::CairoRenderer(
        size_t w, size_t h, const Complex& center, double pixel_res,
        const RGBColor& color)
    : xmin_(center.real() - w/(2*pixel_res)),
      xmax_(center.real() + w/(2*pixel_res)),
      ymin_(center.imag() - h/(2*pixel_res)),
      ymax_(center.imag() + h/(2*pixel_res)),
      surface_(Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, w, h)),
      ctx_(Cairo::Context::create(surface_))
{
    ctx_->scale(pixel_res, pixel_res);
    ctx_->translate(w/(2*pixel_res), h/(2*pixel_res));
    ctx_->translate(center.real(), center.imag());

    ctx_->set_fill_rule(Cairo::FILL_RULE_EVEN_ODD);

    ctx_->rectangle(xmin_, ymin_, xmax_-xmin_, ymax_-ymin_);
    ctx_->set_source_rgb(color.r_, color.g_, color.b_);
    ctx_->fill();
}

void
CairoRenderer::save(const std::string& filename) const {
    surface_->write_to_png(filename);
}

} // apollonian
