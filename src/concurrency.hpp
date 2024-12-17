/* SPDX-License-Identifier: GPL-3.0-only
 *
 * Copyright 2024 Darsh Ranjan.
 *
 * This file is part of super-apollonian-cpp.
 */

#ifndef CONCURRENCY_HPP
#define CONCURRENCY_HPP

#include <mutex>

namespace apollonian {

class grid_dispatch {
public:
    void run();

protected:
    grid_dispatch(
        int num_threads,
        int total_cols, int total_rows,
        int cell_cols, int cell_rows);
    virtual ~grid_dispatch();

    virtual void run_cell(int col0, int row0, int cols, int rows,
                          std::mutex& run_mutex) = 0;

private:
    bool next_cell(int& col0, int& row0);
    void do_work();

private:
    int num_threads_;
    int total_cols_;
    int total_rows_;
    int cell_cols_;
    int cell_rows_;

    int col0_;
    int row0_;

    std::mutex dispatch_mutex_;
    std::mutex run_mutex_;
};

} // apollonian

#endif // CONCURRENCY_HPP
