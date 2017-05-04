#include "graphics.hpp"

#include <cmath>

using apollonian::RGBColor;

namespace {

inline double square(double x) {
    return x*x;
}

double compute_circle_boundary_fraction(
        double xc, double yc, double r,
        int x, int y, double s)
{
    double t = std::hypot(x - xc, y - yc);
    if (r + s <= t) return 0;
    if (t + s <= r) return 1;
    double rr = square(r);
    double ss = square(s);
    double tt = square(t);
    if (r + t <= s) return rr/ss;

    return (ss*std::acos((tt - rr + ss)/(2*s*t)) +
            rr*std::acos((tt - ss + rr)/(2*r*t)) -
            0.5*std::sqrt((r + s + t)*(r + s - t)*
                          (r - s + t)*(s - r + t)))/(M_PI*ss);
}

double compute_line_boundary_fraction(
        double a, double b, double c,
        int x, int y, double s)
{
    double t = a*x + b*y + c;
    if (t >= s) return 0;
    if (t <= -s) return 1;
    double ss = square(s);
    double tt = square(t);
    return (ss*std::acos(t/s) - t*std::sqrt(ss - tt))/(M_PI*ss);
}

} // namespace

void
draw_circle(ImageBuffer<RGBColor>& image,
            double xc, double yc, double r,
            const RGBColor& new_color,
            const RGBColor& old_color)
{
    int rows = image.rows();
    int cols = image.cols();

    double s = std::sqrt(0.5);

    int ymin{std::max(0, int(std::ceil(yc - (r+s))))};
    if (ymin >= rows) return;

    int ymax{std::min(rows-1, int(std::floor(yc + (r+s))))};
    if (ymax < 0) return;

    RGBColor diff = new_color - old_color;

    for (int y = ymin; y <= ymax; ++y) {
        double d0 = std::sqrt(std::max(0.0, square(r+s) - square(y - yc)));
        double d1;
        if (r > s) {
            d1 = std::sqrt(std::max(0.0, square(r-s) - square(y - yc)));
        } else {
            d1 = 0;
        }

        int xmin1{std::max(0, int(std::ceil(xc - d1)))};
        int xmax1{std::min(cols-1, int(std::floor(xc + d1)))};

        int xmin0{std::max(0, int(std::ceil(xc - d0)))};
        int xmax0{std::min(cols-1, int(std::floor(xc + d0)))};

        if (xmin1 < xmax1) {
            for (int x = xmin0; x < xmin1; ++x) {
                double a = compute_circle_boundary_fraction(xc, yc, r, x, y, s);
                image(y, x) += diff*a;
            }
            image.fill_row(new_color, y, xmin1, xmax1+1);
            for (int x = xmax1+1; x <= xmax0; ++x) {
                double a = compute_circle_boundary_fraction(xc, yc, r, x, y, s);
                image(y, x) += diff*a;
            }
        } else {
            for (int x = xmin0; x <= xmax0; ++x) {
                double a = compute_circle_boundary_fraction(xc, yc, r, x, y, s);
                image(y, x) += diff*a;
            }
        }
    }
}

void draw_circle_complement(ImageBuffer<RGBColor>& image,
                            double xc, double yc, double r,
                            const RGBColor& new_color,
                            const RGBColor& old_color)
{
    int rows = image.rows();
    int cols = image.cols();

    double s = std::sqrt(0.5);

    int ymin{std::max(0, int(std::ceil(yc - (r-s))))};
    if (ymin >= rows) return;

    int ymax{std::min(rows-1, int(std::floor(yc + (r-s))))};
    if (ymax < 0) return;

    RGBColor diff = new_color - old_color;

    for (int y = 0; y < ymin; ++y) {
        image.fill_row(new_color, y, 0, cols);
    }
    for (int y = ymin; y <= ymax; ++y) {
        double d0 = std::sqrt(std::max(0.0, square(r+s) - square(y - yc)));
        double d1;
        if (r > s) {
            d1 = std::sqrt(std::max(0.0, square(r-s) - square(y - yc)));
        } else {
            d1 = 0;
        }

        int xmin1{std::max(0, int(std::ceil(xc - d1)))};
        int xmax1{std::min(cols-1, int(std::floor(xc + d1)))};

        int xmin0{std::max(0, int(std::ceil(xc - d0)))};
        int xmax0{std::min(cols-1, int(std::floor(xc + d0)))};

        image.fill_row(new_color, y, 0, xmin0);
        if (xmin1 < xmax1) {
            for (int x = xmin0; x < xmin1; ++x) {
                double a = compute_circle_boundary_fraction(xc, yc, r, x, y, s);
                image(y, x) += diff*(1-a);
            }
            for (int x = xmax1+1; x <= xmax0; ++x) {
                double a = compute_circle_boundary_fraction(xc, yc, r, x, y, s);
                image(y, x) += diff*(1-a);
            }
        } else {
            for (int x = xmin0; x <= xmax0; ++x) {
                double a = compute_circle_boundary_fraction(xc, yc, r, x, y, s);
                image(y, x) += diff*(1-a);
            }
        }
        image.fill_row(new_color, y, xmax0+1, cols);
    }
    for (int y = ymax+1; y < rows; ++y) {
        image.fill_row(new_color, y, 0, cols);
    }
}

/* a*x + b*y + c <= 0 */
void
draw_half_plane(ImageBuffer<apollonian::RGBColor>& image,
                double a, double b, double c,
                const apollonian::RGBColor& new_color,
                const apollonian::RGBColor& old_color)
{
    int rows = image.rows();
    int cols = image.cols();

    double d = std::hypot(a, b);
    a /= d;
    b /= d;
    c /= d;

    double s = std::sqrt(0.5);
    double c0 = c - s;
    double c1 = c + s;

    RGBColor diff = new_color - old_color;

    if (b < 0) {
        if (a < 0) {
            int ymin{std::max(0, int(std::ceil(-(c1 + a*(cols-1))/b)))};
            int ymax{std::min(rows, int(std::floor(-c0/b)))};
            for (int y = ymin; y <= ymax; ++y) {
                int xmin0{std::max(0, int(std::ceil(-(c0 + b*y)/a)))};
                int xmin1{std::min(cols, int(std::floor(-(c1 + b*y)/a)))};
                for (int x = xmin0; x <= xmin1; ++x) {
                    double f = compute_line_boundary_fraction(a, b, c, x, y, s);
                    image(y, x) += diff*f;
                }
                image.fill_row(new_color, y, xmin1+1, cols);
            }
            image.fill_rect(new_color, ymax+1, rows, 0, cols);
        } else {
            int ymin{std::max(0, int(std::ceil(-c1/b)))};
            int ymax{std::min(rows, int(std::floor(-(c0 + a*(cols-1))/b)))};
            for (int y = ymin; y <= ymax; ++y) {
                int xmin0{std::min(cols, int(std::floor(-(c0 + b*y)/a)))};
                int xmin1{std::max(0, int(std::ceil(-(c1 + b*y)/a)))};
                image.fill_row(new_color, y, 0, xmin1);
                for (int x = xmin1; x <= xmin0; ++x) {
                    double f = compute_line_boundary_fraction(a, b, c, x, y, s);
                    image(y, x) += diff*f;
                }
            }
            image.fill_rect(new_color, ymax+1, rows, 0, cols);
        }
    } else {
        if (a < 0) {
            int ymin{std::max(0, int(std::ceil(-c1/b)))};
            int ymax{std::min(rows, int(std::floor(-(c0 + a*(cols-1))/b)))};
            image.fill_rect(new_color, 0, ymin, 0, cols);
            for (int y = ymin; y <= ymax; ++y) {
                int xmin0{std::max(0, int(std::ceil(-(c0 + b*y)/a)))};
                int xmin1{std::min(cols, int(std::floor(-(c1 + b*y)/a)))};
                for (int x = xmin0; x <= xmin1; ++x) {
                    double f = compute_line_boundary_fraction(a, b, c, x, y, s);
                    image(y, x) += diff*f;
                }
                image.fill_row(new_color, y, xmin1+1, cols);
            }
        } else {
            int ymin{std::max(0, int(std::ceil(-(c1 + a*(cols-1))/b)))};
            int ymax{std::min(rows, int(std::floor(-c0/b)))};
            image.fill_rect(new_color, 0, ymin, 0, cols);
            for (int y = ymin; y <= ymax; ++y) {
                int xmin0{std::min(cols, int(std::floor(-(c0 + b*y)/a)))};
                int xmin1{std::max(0, int(std::ceil(-(c1 + b*y)/a)))};
                image.fill_row(new_color, y, 0, xmin1);
                for (int x = xmin1; x <= xmin0; ++x) {
                    double f = compute_line_boundary_fraction(a, b, c, x, y, s);
                    image(y, x) += diff*f;
                }
            }
        }
    }
}
