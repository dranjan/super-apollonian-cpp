#ifndef RENDER_HPP
#define RENDER_HPP

#include <cmath>

#include "riemann_sphere.hpp"
#include "color.hpp"
#include "circle.hpp"
#include "box.hpp"
#include "image_buffer.hpp"
#include "graphics.hpp"

namespace apollonian {

class renderer {
public:
    renderer(int w, int h, const dcomplex& center, double res,
             const rgb_color& color);

public:
    void render_circle(const circle& circle, const rgb_color& new_color,
                       const rgb_color& old_color);
    void save(const std::string& filename) const;
    void map(const dcomplex& z, double& col, double& row) const;
    dcomplex unmap(double col, double row) const;

public:
    box bbox_;
    image_buffer<rgb_color> image_;
    dcomplex center_;
    double res_;
};

inline void
renderer::render_circle(const circle& circle, const rgb_color& new_color,
                        const rgb_color& old_color)
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
renderer::map(const dcomplex& z, double& col, double& row) const {
    col = 0.5*image_.cols() + res_*(z.real() - center_.real());
    row = 0.5*image_.rows() + res_*(z.imag() - center_.imag());
}

inline dcomplex
renderer::unmap(double col, double row) const {
    return {center_.real() + (col - 0.5*image_.cols())/res_,
            center_.imag() + (row - 0.5*image_.rows())/res_};
}

} // apollonian

#endif // RENDER_HPP
