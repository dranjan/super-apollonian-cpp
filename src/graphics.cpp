/* In this implementation, we define each pixel to be a square with unit
 * side length centered at the pixel's nominal coordinates. This is not
 * the only possible definition of a pixel, or necessarily the best, but
 * it does result in a very clean and sharp image with no aliasing
 * artifacts.
 *
 * The implementation that follows is pretty technical, but there's
 * nothing really complicated going on here.
 */
#include "graphics.hpp"

#include <cmath>

using apollonian::RGBColor;

using std::min;
using std::max;
using std::acos;
using std::asin;
using std::sqrt;
using std::ceil;
using std::floor;
using std::sin;

namespace {

inline double square(double x) {
    return x*x;
}

/* Compute the area of the intersection of the first quadrant of a
 * circular disk centered at the origin and an axis-aligned rectangle.
 */
double circle_quadrant_area(
        double rr,  /* square of radius */
        double x0, double y0, double x1, double y1)
{
    if (x1 <= 0 || y1 <= 0) {
        return 0;
    }

    x0 = max(x0, 0.0);
    y0 = max(y0, 0.0);

    double x0x0 = square(x0);
    double y0y0 = square(y0);
    double x1x1 = square(x1);
    double y1y1 = square(y1);

    if (x0x0 + y0y0 >= rr) {
        return 0;
    }

    if (x1x1 + y1y1 <= rr) {
        return (x1 - x0)*(y1 - y0);
    }

    double xa;
    double ya;
    if (x1x1 + y0y0 < rr) {
        xa = x1;
        ya = sqrt(rr - x1x1);
    } else {
        ya = y0;
        xa = sqrt(rr - y0y0);
    }

    double xb;
    double yb;
    if (x0x0 + y1y1 < rr) {
        yb = y1;
        xb = sqrt(rr - y1y1);
    } else {
        xb = x0;
        yb = sqrt(rr - x0x0);
    }

    double ta = atan2(ya, xa);
    double tb = atan2(yb, xb);

    double area = 0.5*(rr*(tb - ta) - xa*yb  + xb*ya)
        + (xa - x0)*(yb - y0)
        - 0.5*(xa - xb)*(yb - ya);
    return area;
}

/* Compute the area of the intersection of a circular disk and square pixel
 * with sides of unit length.
 */
double compute_circle_boundary_fraction(
        double xc, double yc, double r,
        double x0, double y0)
{
    x0 -= xc;
    y0 -= yc;

    double rr = r*r;

    double x1 = x0 + 1;
    double y1 = y0 + 1;

    return circle_quadrant_area(rr, x0, y0, x1, y1) +
        circle_quadrant_area(rr, -y1, x0, -y0, x1) +
        circle_quadrant_area(rr, -x1, -y1, -x0, -y0) +
        circle_quadrant_area(rr, y0, -x1, y1, -x0);
}

/* Compute the area of the intersection of a half plane and square pixel
 * with sides of unit length.
 */
double compute_line_boundary_fraction(
        double a, double b, double c,
        double x0, double y0)
{
    double d = a*a + b*b;
    x0 -= a*c/d;
    y0 -= b*c/d;

    if (a < 0) {
        if (b < 0) {
            x0 = -x0 - 1;
            y0 = -y0 - 1;
            a = -a;
            b = -b;
        } else {
            double xt = x0;
            x0 = y0;
            y0 = -xt - 1;
            double t = a;
            a = b;
            b = -t;
        }
    } else if (b < 0) {
        double yt = y0;
        y0 = x0;
        x0 = -yt - 1;
        double t = b;
        b = a;
        a = -t;
    }

    if (a*x0 + b*y0 >= 0) {
        return 0;
    }

    double x1 = x0 + 1;
    double y1 = y0 + 1;

    if (a*x1 + b*y1 <= 0) {
        return 1;
    }

    double xa;
    double ya;
    if (a*x1 + b*y0 <= 0) {
        xa = x1;
        ya = -a*x1/b;
    } else {
        ya = y0;
        xa = -b*y0/a;
    }

    double xb;
    double yb;
    if (a*x0 + b*y1 <= 0) {
        yb = y1;
        xb = -b*y1/a;
    } else {
        xb = x0;
        yb = -a*x0/b;
    }

    return (xa - x0)*(yb - y0)
           - 0.5*(xa - xb)*(yb - ya);
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

    double s = sqrt(0.5);

    int ymin{max(0, int(ceil(yc - 0.5 - (r+s))))};
    if (ymin >= rows) return;

    int ymax{min(rows-1, int(floor(yc - 0.5 + (r+s))))};
    if (ymax < 0) return;

    RGBColor diff = new_color - old_color;

    for (int y = ymin; y <= ymax; ++y) {
        double d0 = sqrt(max(0.0, square(r+s) - square(y - yc + 0.5)));
        double d1;
        if (r > s) {
            d1 = sqrt(max(0.0, square(r-s) - square(y - yc + 0.5)));
        } else {
            d1 = 0;
        }

        int xmin1{max(0, int(ceil(xc - 0.5 - d1)))};
        int xmax1{min(cols-1, int(floor(xc - 0.5 + d1)))};

        int xmin0{max(0, int(ceil(xc - 0.5 - d0)))};
        int xmax0{min(cols-1, int(floor(xc - 0.5 + d0)))};

        if (xmin1 < xmax1) {
            for (int x = xmin0; x < xmin1; ++x) {
                double a = compute_circle_boundary_fraction(xc, yc, r, x, y);
                image(y, x) += diff*a;
            }
            image.fill_row(new_color, y, xmin1, xmax1+1);
            for (int x = xmax1+1; x <= xmax0; ++x) {
                double a = compute_circle_boundary_fraction(xc, yc, r, x, y);
                image(y, x) += diff*a;
            }
        } else {
            for (int x = xmin0; x <= xmax0; ++x) {
                double a = compute_circle_boundary_fraction(xc, yc, r, x, y);
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

    double s = sqrt(0.5);

    int ymin{max(0, int(ceil(yc - 0.5 - (r+s))))};
    if (ymin >= rows) return;

    int ymax{min(rows-1, int(floor(yc - 0.5 + (r+s))))};
    if (ymax < 0) return;

    RGBColor diff = new_color - old_color;

    for (int y = 0; y < ymin; ++y) {
        image.fill_row(new_color, y, 0, cols);
    }
    for (int y = ymin; y <= ymax; ++y) {
        double d0 = sqrt(max(0.0, square(r+s) - square(y - yc + 0.5)));
        double d1;
        if (r > s) {
            d1 = sqrt(max(0.0, square(r-s) - square(y - yc + 0.5)));
        } else {
            d1 = 0;
        }

        int xmin1{max(0, int(ceil(xc - 0.5 - d1)))};
        int xmax1{min(cols-1, int(floor(xc - 0.5 + d1)))};

        int xmin0{max(0, int(ceil(xc - 0.5 - d0)))};
        int xmax0{min(cols-1, int(floor(xc - 0.5 + d0)))};

        image.fill_row(new_color, y, 0, xmin0);
        if (xmin1 < xmax1) {
            for (int x = xmin0; x < xmin1; ++x) {
                double a = compute_circle_boundary_fraction(xc, yc, r, x, y);
                image(y, x) += diff*(1-a);
            }
            for (int x = xmax1+1; x <= xmax0; ++x) {
                double a = compute_circle_boundary_fraction(xc, yc, r, x, y);
                image(y, x) += diff*(1-a);
            }
        } else {
            for (int x = xmin0; x <= xmax0; ++x) {
                double a = compute_circle_boundary_fraction(xc, yc, r, x, y);
                image(y, x) += diff*(1-a);
            }
        }
        image.fill_row(new_color, y, xmax0+1, cols);
    }
    for (int y = ymax+1; y < rows; ++y) {
        image.fill_row(new_color, y, 0, cols);
    }
}

void
draw_half_plane(ImageBuffer<apollonian::RGBColor>& image,
                double a, double b, double c,
                const apollonian::RGBColor& new_color,
                const apollonian::RGBColor& old_color)
{
    int rows = image.rows();
    int cols = image.cols();

    double d = hypot(a, b);
    a /= d;
    b /= d;
    c /= d;

    RGBColor diff = new_color - old_color;

    if (a == 0) {
        int y = int(floor(-c/b));
        if (b < 0) {
            image.fill_rect(new_color, y+1, rows, 0, cols);
        } else {
            image.fill_rect(new_color, 0, y, 0, cols);
        }
        if (0 <= y && y < rows) {
            for (int x = 0; x < cols; ++x) {
                double f = compute_line_boundary_fraction(a, b, c, x, y);
                image(y, x) += diff*f;
            }
        }
    } else if (a < 0) {
        if (b < 0) {
            for (int y = 0; y < rows; ++y) {
                int x0 = max(0, int(floor(-(c + b*(y+1))/a)));
                int x1 = min(cols, int(ceil(-(c + b*y)/a)));
                for (int x = x0; x < x1; ++x) {
                    double f = compute_line_boundary_fraction(a, b, c, x, y);
                    image(y, x) += diff*f;
                }
                image.fill_row(new_color, y, x1, cols);
            }
        } else {
            for (int y = 0; y < rows; ++y) {
                int x0 = max(0, int(floor(-(c + b*y)/a)));
                int x1 = min(cols, int(ceil(-(c + b*(y+1))/a)));
                for (int x = x0; x < x1; ++x) {
                    double f = compute_line_boundary_fraction(a, b, c, x, y);
                    image(y, x) += diff*f;
                }
                image.fill_row(new_color, y, x1, cols);
            }
        }
    } else {
        if (b < 0) {
            for (int y = 0; y < rows; ++y) {
                int x0 = max(0, int(floor(-(c + b*y)/a)));
                int x1 = min(cols, int(ceil(-(c + b*(y+1))/a)));
                image.fill_row(new_color, y, 0, x0);
                for (int x = x0; x < x1; ++x) {
                    double f = compute_line_boundary_fraction(a, b, c, x, y);
                    image(y, x) += diff*f;
                }
            }
        } else {
            for (int y = 0; y < rows; ++y) {
                int x0 = max(0, int(floor(-(c + b*(y+1))/a)));
                int x1 = min(cols, int(ceil(-(c + b*y)/a)));
                image.fill_row(new_color, y, 0, x0);
                for (int x = x0; x < x1; ++x) {
                    double f = compute_line_boundary_fraction(a, b, c, x, y);
                    image(y, x) += diff*f;
                }
            }
        }
    }
}
