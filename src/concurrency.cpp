/* SPDX-License-Identifier: GPL-3.0-only
 *
 * Copyright 2024 Darsh Ranjan.
 *
 * This file is part of super-apollonian-cpp.
 */

#include "concurrency.hpp"

#include <iostream>
#include <thread>
#include <vector>

namespace apollonian {

grid_dispatch::grid_dispatch(
    int num_threads,
        int total_cols, int total_rows,
        int cell_cols, int cell_rows)
    : num_threads_(num_threads),
      total_cols_(total_cols), total_rows_(total_rows),
      cell_cols_(cell_cols), cell_rows_(cell_rows),
      col0_(0), row0_(0)
{
}

grid_dispatch::~grid_dispatch() {
}

void grid_dispatch::run() {
    std::vector<std::thread> workers;
    for (int k = 0; k < num_threads_; ++k) {
        workers.emplace_back(&grid_dispatch::do_work, this);
    }
    for (auto& worker : workers) worker.join();
}

void grid_dispatch::do_work() {
    int col0;
    int row0;
    while (next_cell(col0, row0)) {
        run_cell(col0, row0, cell_cols_, cell_rows_, run_mutex_);
    }
}

bool grid_dispatch::next_cell(int& col0, int& row0) {
    std::unique_lock<std::mutex> lock(dispatch_mutex_);
    if (row0_ >= total_rows_) {
        return false;
    }
    col0 = col0_;
    row0 = row0_;
    std::cout << "Dispatch: " << col0 << ", " << row0 << std::endl;
    col0_ += cell_cols_;
    if (col0_ >= total_cols_) {
        col0_ = 0;
        row0_ += cell_rows_;
    }
    return true;
}

} // apollonian
