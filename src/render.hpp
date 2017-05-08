#ifndef RENDER_HPP
#define RENDER_HPP

#include <cmath>

#include <cairomm/context.h>
#include <cairomm/surface.h>

#include "riemann_sphere.hpp"
#include "color.hpp"
#include "circle.hpp"
#include "box.hpp"
#include "image_buffer.hpp"
#include "graphics.hpp"

namespace apollonian {

class Renderer {
public:
    Renderer(int w, int h, const Complex& center, double res,
             const RGBColor& color);

public:
    void render_circle(const Circle& circle, const RGBColor& new_color,
                       const RGBColor& old_color);
    void save(const std::string& filename) const;
    void map(const Complex& z, double& col, double& row) const;
    Complex unmap(double col, double row) const;

public:
    Box bbox_;
    ImageBuffer<RGBColor> image_;
    Complex center_;
    double res_;
};

inline void
Renderer::render_circle(const Circle& circle, const RGBColor& new_color,
                        const RGBColor& old_color)
{
    if (circle.v00_ == 0) {
        double a = 2*circle.v01_.real()/res_;
        double b = 2*circle.v01_.imag()/res_;
        double c = circle.v11_ + 2*(circle.v01_*center_).real()
                 - 0.5*a*image_.cols() - 0.5*b*image_.rows();
        draw_half_plane(image_, a, b, c, new_color, old_color);
    } else {
        double xc;
        double yc;
        map(circle.center(), xc, yc);
        double r = circle.radius()*res_;
        if (r < 0) {
            draw_circle_complement(image_, xc, yc, -r, new_color, old_color);
        } else {
            draw_circle(image_, xc, yc, r, new_color, old_color);
        }
    }
}

inline void
Renderer::map(const Complex& z, double& col, double& row) const {
    col = 0.5*image_.cols() + res_*(z.real() - center_.real());
    row = 0.5*image_.rows() + res_*(z.imag() - center_.imag());
}

inline Complex
Renderer::unmap(double row, double col) const {
    return {center_.real() + (col - 0.5*image_.cols())/res_,
            center_.imag() + (row - 0.5*image_.rows())/res_};
}

} // apollonian

#endif // RENDER_HPP
