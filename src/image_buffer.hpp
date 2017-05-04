#ifndef IMAGE_BUFFER_HPP
#define IMAGE_BUFFER_HPP

#include <cstring>

#include <vector>
#include <algorithm>

template <typename Pixel>
void fill_row(const Pixel& value, Pixel* begin, Pixel* end) {
    if (end <= begin) return;

    Pixel* p = begin;
    *(p++) = value;
    while (p < end) {
        Pixel* q = p + (p - begin);
        if (q > end) q = end;
        memcpy(reinterpret_cast<void*>(p),
               reinterpret_cast<const void*>(p - (q - p)),
               (q - p)*sizeof(*p));
        p = q;
    }
}

template <typename Pixel>
void fill_rect(const Pixel& value, Pixel* data,
               int rows, int cols, int stride)
{
    if (rows <= 0) return;

    fill_row(value, data, data + cols);

    char* p = reinterpret_cast<char*>(data);
    while (--rows) {
        memcpy(p + stride, p, cols*sizeof(Pixel));
        p += stride;
    }
}

template <typename Pixel>
class ImageBuffer {
public:
    ImageBuffer(int rows, int cols);

    const Pixel& operator () (int row, int col) const;
    Pixel& operator () (int row, int col);

    const Pixel* operator [] (int row) const;
    Pixel* operator [] (int row);

    void fill_row(const Pixel& value, int row,
                  int col_begin, int col_end);
    void fill_rect(const Pixel& value, int row_begin, int row_end,
                   int col_begin, int col_end);
    void fill(const Pixel& value);

    int rows() const;
    int cols() const;

private:
    int rows_;
    int cols_;
    std::vector<Pixel> data_;
};

template <typename Pixel>
ImageBuffer<Pixel>::ImageBuffer(int rows, int cols)
    : rows_{rows}, cols_{cols}, data_{size_t(rows*cols)}
{
}

template <typename Pixel>
const Pixel&
ImageBuffer<Pixel>::operator () (int row, int col) const {
    return data_[row*cols_ + col];
}

template <typename Pixel>
Pixel&
ImageBuffer<Pixel>::operator () (int row, int col) {
    return data_[row*cols_ + col];
}

template <typename Pixel>
const Pixel* ImageBuffer<Pixel>::operator [] (int row) const {
    return data_.data() + row*cols_;
}

template <typename Pixel>
Pixel* ImageBuffer<Pixel>::operator [] (int row) {
    return data_.data() + row*cols_;
}

template <typename Pixel>
void ImageBuffer<Pixel>::fill_row(
        const Pixel& value, int row,
        int col_begin, int col_end)
{
    if (row < 0 || row >= rows_) return;
    if (col_begin < 0) col_begin = 0;
    if (col_end > cols_) col_end = cols_;

    Pixel* row_ptr = operator [] (row);
    ::fill_row(value, row_ptr + col_begin, row_ptr + col_end);
}

template <typename Pixel>
void ImageBuffer<Pixel>::fill_rect(
        const Pixel& value,
        int row_begin, int row_end,
        int col_begin, int col_end)
{
    if (row_begin < 0) row_begin = 0;
    if (row_end > rows_) row_end = rows_;
    if (col_begin < 0) col_begin = 0;
    if (col_end > cols_) col_end = cols_;

    ::fill_rect(value, data_.data() + row_begin*cols_ + col_begin,
                row_end - row_begin, col_end - col_begin,
                cols_*sizeof(Pixel));
}

template <typename Pixel>
void ImageBuffer<Pixel>::fill(const Pixel& value) {
    ::fill_row(value, data_.data(), data_.data() + data_.size());
}

template <typename Pixel>
int ImageBuffer<Pixel>::rows() const {
    return rows_;
}

template <typename Pixel>
int ImageBuffer<Pixel>::cols() const {
    return cols_;
}


#endif // IMAGE_BUFFER_HPP
