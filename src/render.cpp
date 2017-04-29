#include "render.hpp"

namespace apollonian {

CairoRenderer::CairoRenderer(
        size_t w, size_t h, const Complex& center, double pixel_res,
        const RGBColor& color)
    : bbox_{make_box(center, w/pixel_res, h/pixel_res)},
      surface_{Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, w, h)},
      ctx_{Cairo::Context::create(surface_)}
{
    ctx_->scale(pixel_res, pixel_res);
    ctx_->translate(w/(2*pixel_res), h/(2*pixel_res));
    ctx_->translate(center.real(), center.imag());

    ctx_->set_fill_rule(Cairo::FILL_RULE_EVEN_ODD);

    ctx_->rectangle(bbox_.xmin, bbox_.ymin,
                    bbox_.xmax - bbox_.xmin, bbox_.ymax - bbox_.ymin);
    ctx_->set_source_rgb(color.r_, color.g_, color.b_);
    ctx_->fill();
}

void
CairoRenderer::save(const std::string& filename) const {
    surface_->write_to_png(filename);
}

} // apollonian
