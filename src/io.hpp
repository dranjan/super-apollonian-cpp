/* SPDX-License-Identifier: GPL-3.0-only
 *
 * Copyright 2024 Darsh Ranjan.
 *
 * This file is part of super-apollonian-cpp.
 */

#ifndef IO_HPP
#define IO_HPP

#include <string>

#include "color.hpp"
#include "image_buffer.hpp"

namespace apollonian {

void save_image(const image_buffer<rgb_color>& image,
                const std::string& filename);

} // apollonian

#endif // IO_HPP
