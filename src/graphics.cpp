#include <cmath>

#include "graphics.hpp"

using apollonian::RGBColor;

inline double square(double x) {
    return x*x;
}

double compute_boundary_fraction(
        double xc, double yc, double r,
        int x, int y, double s)
{
    double t = std::hypot(x - xc, y - yc);
    if (r + s <= t) return 0;
    if (t + s <= r) return 1;
    double rr = square(r);
    double ss = square(s);
    double tt = square(t);
    return (ss*std::acos((tt - rr + ss)/(2*s*t)) +
            rr*std::acos((tt - ss + rr)/(2*r*t)) -
            0.5*std::sqrt((r + s + t)*(r + s - t)*
                          (r - s + t)*(s - r + t)))/(M_PI*ss);
}

void
draw_circle(ImageBuffer<RGBColor>& image,
            double xc, double yc, double r,
            const RGBColor& color)
{
    int rows = image.rows();
    int cols = image.cols();

    double s = std::sqrt(0.5);

    int ymin{std::max(0, int(std::ceil(yc - (r+s))))};
    if (ymin >= rows) return;

    int ymax{std::min(rows-1, int(std::floor(yc + (r+s))))};
    if (ymax < 0) return;

    for (int y = ymin; y <= ymax; ++y) {
        double d0 = std::sqrt(std::max(0.0, square(r+s) - square(y - yc)));
        double d1 = std::sqrt(std::max(0.0, square(r-s) - square(y - yc)));

        int xmin1{std::max(0, int(std::ceil(xc - d1)))};
        int xmax1{std::min(cols-1, int(std::floor(xc + d1)))};

        int xmin0{std::max(0, int(std::ceil(xc - d0)))};
        int xmax0{std::min(cols-1, int(std::floor(xc + d0)))};

        if (xmin1 < xmax1) {
            for (int x = xmin0; x < xmin1; ++x) {
                double a = compute_boundary_fraction(xc, yc, r, x, y, s);
                const RGBColor& pixel = image(y, x);
                image(y, x) = pixel.blend(color, a);
            }
            image.fill_row(color, y, xmin1, xmax1+1);
            for (int x = xmax1+1; x <= xmax0; ++x) {
                double a = compute_boundary_fraction(xc, yc, r, x, y, s);
                const RGBColor& pixel = image(y, x);
                image(y, x) = pixel.blend(color, a);
            }
        } else {
            for (int x = xmin0; x <= xmax0; ++x) {
                double a = compute_boundary_fraction(xc, yc, r, x, y, s);
                const RGBColor& pixel = image(y, x);
                image(y, x) = pixel.blend(color, a);
            }
        }
    }
}
