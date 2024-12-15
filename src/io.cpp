/* SPDX-License-Identifier: GPL-3.0-only
 *
 * Copyright 2024 Darsh Ranjan.
 *
 * This file is part of super-apollonian-cpp.
 */

#include "io.hpp"

#include <algorithm>

#include <cairomm/context.h>
#include <cairomm/surface.h>

namespace apollonian {

template <typename T>
inline T clamp(const T& value, const T& min, const T& max) {
    return std::min(max, std::max(min, value));
}

inline uint8_t
get_component(int32_t value) {
    if (value < 0) return 0;
    return value >> 23;
}

inline unsigned char*
write_pixel(const rgb_color& pixel, unsigned char* p) {
    *((uint32_t*)p) = (get_component(pixel.b_) << 0) +
                      (get_component(pixel.g_) << 8) +
                      (get_component(pixel.r_) << 16) +
                      (0xff << 24);
    return p + 4;
}

void save_image(const image_buffer<rgb_color>& image,
                const std::string& filename)
{
    int rows = image.rows();
    int cols = image.cols();

    Cairo::RefPtr<Cairo::ImageSurface> surface =
        Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, cols, rows);

    unsigned char* data = surface->get_data();
    int stride = surface->get_stride();
    for (int row = 0; row < rows; ++row) {
        unsigned char* p = data + (rows - row - 1)*stride;
        for (int col = 0; col < cols; ++col) {
            p = write_pixel(image(row, col), p);
        }
    }
    surface->write_to_png(filename);
}

} // apollonian
