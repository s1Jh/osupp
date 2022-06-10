#pragma once

#include <array>
#include <cstddef>

#include <type_traits>
#include "define.hpp"

NS_BEGIN

template<typename T, unsigned int w, unsigned int h> requires std::is_arithmetic_v<T>
class Mat {
    friend class CollumnView;

    friend class RowView;

public:
    inline constexpr explicit Mat(T fill = 0) {
        for (int x = 0; x < w; x++)
            for (int y = 0; y < h; y++)
                m_Values[x][y] = fill;
    }

    inline constexpr explicit Mat(std::array<float, w * h> data) {
        for (int x = 0; x < w; x++)
            for (int y = 0; y < h; y++)
                m_Values[x][y] = data[y * w + x];
    }

    Mat(const Mat& other) {
        for (int x = 0; x < w; x++)
            for (int y = 0; y < h; y++)
                m_Values[x][y] = other.m_Values[x][y];
    }

    Mat(const Mat&& other)  noexcept {
        for (int x = 0; x < w; x++)
            for (int y = 0; y < h; y++)
                m_Values[x][y] = other.m_Values[x][y];
    }

    inline Mat& operator=(const Mat& other) {
        if (this == &other)
            return *this;

        for (int x = 0; x < w; x++)
            for (int y = 0; y < h; y++)
                m_Values[x][y] = other.m_Values[x][y];

        return *this;
    }

    inline Mat& operator=(Mat&& other) noexcept {
        for (int x = 0; x < w; x++)
            for (int y = 0; y < h; y++)
                m_Values[x][y] = other.m_Values[x][y];

        return *this;
    }

    class CollumnView {
    public:
        CollumnView(Mat<T, w, h> &original, size_t column) :
                m_Original(original), m_uCollumn(column) {}

        inline T &operator[](int row) {
            return m_Original.m_Values[row % h][m_uCollumn];
        }

    private:
        Mat<T, w, h> &m_Original;
        size_t m_uCollumn;
    };

    class ConstCollumnView {
    public:
        ConstCollumnView(const Mat<T, w, h> &original, size_t column) :
                m_Original(original), m_uCollumn(column) {}

        inline T operator[](int row) const {
            return m_Original.m_Values[row % h][m_uCollumn];
        }

    private:
        const Mat<T, w, h> &m_Original;
        size_t m_uCollumn;
    };

    class RowView {
    public:
        RowView(Mat<T, w, h> &original, size_t row) :
                m_Original(original), m_uRow(row) {}

        inline T &operator[](int col) {
            return m_Original.m_Values[m_uRow][col % w];
        }

    private:
        Mat<T, w, h> &m_Original;
        size_t m_uRow;
    };

    class ConstRowView {
    public:
        ConstRowView(const Mat<T, w, h> &original, size_t row) :
                m_Original(original), m_uRow(row) {}

        inline T operator[](int col) const {
            return m_Original.m_Values[m_uRow][col % w];
        }

    private:
        const Mat<T, w, h> &m_Original;
        size_t m_uRow;
    };

    inline CollumnView column_view(size_t col) {
        return CollumnView(*this, col % w);
    }

    inline ConstCollumnView column_view(size_t col) const {
        return ConstCollumnView(*this, col % w);
    }

    inline RowView row_view(size_t row) {
        return RowView(*this, row % h);
    }

    inline ConstRowView row_view(size_t row) const {
        return ConstRowView(*this, row % h);
    }

    inline RowView operator[](size_t row) {
        return row_view(row);
    }

    inline ConstRowView operator[](size_t row) const {
        return row_view(row);
    }

    T *GetPtr() {
        return (T *) m_Values;
    }

    const T *GetCPtr() const {
        return (T *) m_Values;
    }

private:
    // [columns][rows]
    T m_Values[h][w];
};

template<typename T, size_t I>
using SquareMatrix = Mat<T, I, I>;

template<typename T>
using Mat1 = SquareMatrix<T, 1>;
using Mat1f = Mat1<float>;
using Mat1u = Mat1<unsigned int>;
using Mat1i = Mat1<int>;
using Mat1d = Mat1<double>;

template<typename T>
using Mat1x1 = SquareMatrix<T, 1>;
using Mat1x1f = Mat1x1<float>;
using Mat1x1d = Mat1x1<double>;
using Mat1x1u = Mat1x1<unsigned int>;
using Mat1x1i = Mat1x1<int>;

template<typename T>
using Mat1x2 = Mat<T, 1, 2>;
using Mat1x2f = Mat1x2<float>;
using Mat1x2d = Mat1x2<double>;
using Mat1x2u = Mat1x2<unsigned int>;
using Mat1x2i = Mat1x2<int>;

template<typename T>
using Mat1x3 = Mat<T, 1, 3>;
using Mat1x3f = Mat1x3<float>;
using Mat1x3d = Mat1x3<double>;
using Mat1x3u = Mat1x3<unsigned int>;
using Mat1x3i = Mat1x3<int>;

template<typename T>
using Mat1x4 = Mat<T, 1, 4>;
using Mat1x4f = Mat1x4<float>;
using Mat1x4d = Mat1x4<double>;
using Mat1x4u = Mat1x4<unsigned int>;
using Mat1x4i = Mat1x4<int>;

template<typename T>
using Mat2x1 = Mat<T, 2, 1>;
using Mat2x1f = Mat2x1<float>;
using Mat2x1d = Mat2x1<double>;
using Mat2x1u = Mat2x1<unsigned int>;
using Mat2x1i = Mat2x1<int>;

template<typename T>
using Mat2 = SquareMatrix<T, 2>;
template<typename T>
using Mat2 = SquareMatrix<T, 2>;
using Mat2f = Mat2<float>;
using Mat2d = Mat2<double>;
using Mat2u = Mat2<unsigned int>;
using Mat2i = Mat2<int>;

template<typename T>
using Mat2x2 = SquareMatrix<T, 2>;
using Mat2x2f = Mat2x2<float>;
using Mat2x2d = Mat2x2<double>;
using Mat2x2u = Mat2x2<unsigned int>;
using Mat2x2i = Mat2x2<int>;

template<typename T>
using Mat2x3 = Mat<T, 2, 3>;
using Mat2x3f = Mat2x3<float>;
using Mat2x3d = Mat2x3<double>;
using Mat2x3u = Mat2x3<unsigned int>;
using Mat2x3i = Mat2x3<int>;

template<typename T>
using Mat2x4 = Mat<T, 2, 4>;
using Mat2x4f = Mat2x4<float>;
using Mat2x4d = Mat2x4<double>;
using Mat2x4u = Mat2x4<unsigned int>;
using Mat2x4i = Mat2x4<int>;

template<typename T>
using Mat3x1 = Mat<T, 3, 1>;
using Mat3x1f = Mat3x1<float>;
using Mat3x1d = Mat3x1<double>;
using Mat3x1u = Mat3x1<unsigned int>;
using Mat3x1i = Mat3x1<int>;

template<typename T>
using Mat3x2 = Mat<T, 3, 2>;
using Mat3x2f = Mat3x2<float>;
using Mat3x2d = Mat3x2<double>;
using Mat3x2u = Mat3x2<unsigned int>;
using Mat3x2i = Mat3x2<int>;

template<typename T>
using Mat3 = SquareMatrix<T, 3>;
using Mat3f = Mat3<float>;
using Mat3d = Mat3<double>;
using Mat3u = Mat3<unsigned int>;
using Mat3i = Mat3<int>;

template<typename T>
using Mat3x3 = SquareMatrix<T, 3>;
using Mat3x3f = Mat3x3<float>;
using Mat3x3d = Mat3x3<double>;
using Mat3x3u = Mat3x3<unsigned int>;
using Mat3x3i = Mat3x3<int>;

template<typename T>
using Mat3x4 = Mat<T, 3, 4>;
using Mat3x4f = Mat3x4<float>;
using Mat3x4d = Mat3x4<double>;
using Mat3x4u = Mat3x4<unsigned int>;
using Mat3x4i = Mat3x4<int>;

template<typename T>
using Mat4x1 = Mat<T, 4, 1>;
using Mat4x1f = Mat4x1<float>;
using Mat4x1d = Mat4x1<double>;
using Mat4x1u = Mat4x1<unsigned int>;
using Mat4x1i = Mat4x1<int>;

template<typename T>
using Mat4x2 = Mat<T, 4, 2>;
using Mat4x2f = Mat4x2<float>;
using Mat4x2d = Mat4x2<double>;
using Mat4x2u = Mat4x2<unsigned int>;
using Mat4x2i = Mat4x2<int>;

template<typename T>
using Mat4x3 = Mat<T, 4, 3>;
using Mat4x3f = Mat4x3<float>;
using Mat4x3d = Mat4x3<double>;
using Mat4x3u = Mat4x3<unsigned int>;
using Mat4x3i = Mat4x3<int>;

template<typename T>
using Mat4 = SquareMatrix<T, 4>;
using Mat4f = Mat4<float>;
using Mat4d = Mat4<double>;
using Mat4u = Mat4<unsigned int>;
using Mat4i = Mat4<int>;

template<typename T>
using Mat4x4 = SquareMatrix<T, 4>;
using Mat4x4f = Mat4x4<float>;
using Mat4x4d = Mat4x4<double>;
using Mat4x4u = Mat4x4<unsigned int>;
using Mat4x4i = Mat4x4<int>;

template<typename T>
constexpr Mat2<T> MAT2_NO_TRANSFORM = Mat2<T>({
                                                      1.f, 0.f,
                                                      0.f, 1.f
                                              });

template<typename T>
constexpr Mat3<T> MAT3_NO_TRANSFORM = Mat3<T>({
                                                      1.f, 0.f, 0.f,
                                                      0.f, 1.f, 0.f,
                                                      0.f, 0.f, 1.f
                                              });

template<typename T>
constexpr Mat4<T> MAT4_NO_TRANSFORM = Mat4<T>({
                                                      1.f, 0.f, 0.f, 0.f,
                                                      0.f, 1.f, 0.f, 0.f,
                                                      0.f, 0.f, 1.f, 0.f,
                                                      0.f, 0.f, 0.f, 1.f
                                              });

template<typename L, int Lw, int Lh, typename R, int Rw, int Rh>
bool operator==(const Mat<L, Lw, Lh> &left, const Mat<R, Rw, Rh> &right) {
    if (Lw != Rw || Lh != Rh)
        return false;

    for (int y = 0; y < Rw; y++)
        for (int x = 0; x < Lh; x++) {
            if (left[y][x] != right[y][x])
                return false;
        }

    return true;
}

template<typename L, int Lw, int Lh, typename R, int Rw, int Rh>
bool operator!=(const Mat<L, Lw, Lh> &left, const Mat<R, Rw, Rh> &right) {
    if (Lw != Rw || Lh != Rh)
        return true;

    for (int y = 0; y < Rw; y++)
        for (int x = 0; x < Lh; x++) {
            if (left[y][x] != right[y][x])
                return true;
        }

    return false;
}

// for assymetric operations or otherwise not expanded operations
template<typename L, int Lw, int Lh, typename R, int Rw, int Rh>
typename std::enable_if<((Rw != Lw || Rh != Lh) || Lw > 4), Mat<R, Rw, Lh>>::type
operator*(const Mat<L, Lw, Lh> &left, const Mat<R, Rw, Rh> &right) {
    static_assert(Rh == Lw, "Invalid matrix multiplication parameters: left width and right height don't match!");

    Mat<R, Rw, Lh> result;

    for (int y = 0; y < Rw; y++)
        for (int x = 0; x < Lh; x++) {
            R accum = 0;
            for (int z = 0; z < Rh; z++)
                accum += left.row_view(y)[z] * right.column_view(x)[z];

            result[y][x] = accum;
        }

    return result;
}

template<typename L, typename R>
Mat2<L> operator*(const Mat2<L> &left, const Mat2<R> &right) {
    Mat2<L> res;

    res[0][0] = left[0][0] * right[0][0] + left[0][1] * right[1][0];
    res[0][1] = left[0][0] * right[0][0] + left[0][1] * right[1][1];
    res[1][0] = left[1][0] * right[0][1] + left[1][1] * right[1][0];
    res[1][1] = left[1][0] * right[0][1] + left[1][1] * right[1][1];

    return res;
}

template<typename L, typename R>
Mat3<L> operator*(const Mat3<L> &left, const Mat3<R> &right) {
    Mat3<L> res;

    res[0][0] = left[0][0] * right[0][0] + left[0][1] * right[1][0] + left[0][2] * right[2][0];
    res[0][1] = left[0][0] * right[0][1] + left[0][1] * right[1][1] + left[0][2] * right[2][1];
    res[0][2] = left[0][0] * right[0][2] + left[0][1] * right[1][2] + left[0][2] * right[2][2];
    res[1][0] = left[1][0] * right[0][0] + left[1][1] * right[1][0] + left[1][2] * right[2][0];
    res[1][1] = left[1][0] * right[0][1] + left[1][1] * right[1][1] + left[1][2] * right[2][1];
    res[1][2] = left[1][0] * right[0][2] + left[1][1] * right[1][2] + left[1][2] * right[2][2];
    res[2][0] = left[2][0] * right[0][0] + left[2][1] * right[1][0] + left[2][2] * right[2][0];
    res[2][1] = left[2][0] * right[0][1] + left[2][1] * right[1][1] + left[2][2] * right[2][1];
    res[2][2] = left[2][0] * right[0][2] + left[2][1] * right[1][2] + left[2][2] * right[2][2];

    return res;
}

template<typename L, typename R>
Mat4<L> operator*(const Mat4<L> &left, const Mat4<R> &right) {
    Mat4<L> res;

    res[0][0] = left[0][0] * right[0][0] + left[0][1] * right[1][0] +
                left[0][2] * right[2][0] + left[0][3] * right[3][0];
    res[0][1] = left[0][0] * right[0][1] + left[0][1] * right[1][1] +
                left[0][2] * right[2][1] + left[0][3] * right[3][1];
    res[0][2] = left[0][0] * right[0][2] + left[0][1] * right[1][2] +
                left[0][2] * right[2][2] + left[0][3] * right[3][2];
    res[0][3] = left[0][0] * right[0][3] + left[0][1] * right[1][3] +
                left[0][2] * right[2][3] + left[0][3] * right[3][3];
    res[1][0] = left[1][0] * right[0][0] + left[1][1] * right[1][0] +
                left[1][2] * right[2][0] + left[1][3] * right[3][0];
    res[1][1] = left[1][0] * right[0][1] + left[1][1] * right[1][1] +
                left[1][2] * right[2][1] + left[1][3] * right[3][1];
    res[1][2] = left[1][0] * right[0][2] + left[1][1] * right[1][2] +
                left[1][2] * right[2][2] + left[1][3] * right[3][2];
    res[1][3] = left[1][0] * right[0][3] + left[1][1] * right[1][3] +
                left[1][2] * right[2][3] + left[1][3] * right[3][3];
    res[2][0] = left[2][0] * right[0][0] + left[2][1] * right[1][0] +
                left[2][2] * right[2][0] + left[2][3] * right[3][0];
    res[2][1] = left[2][0] * right[0][1] + left[2][1] * right[1][1] +
                left[2][2] * right[2][1] + left[2][3] * right[3][1];
    res[2][2] = left[2][0] * right[0][2] + left[2][1] * right[1][2] +
                left[2][2] * right[2][2] + left[2][3] * right[3][2];
    res[2][3] = left[2][0] * right[0][3] + left[2][1] * right[1][3] +
                left[2][2] * right[2][3] + left[2][3] * right[3][3];
    res[3][0] = left[3][0] * right[0][0] + left[3][1] * right[1][0] +
                left[3][2] * right[2][0] + left[3][3] * right[3][0];
    res[3][1] = left[3][0] * right[0][1] + left[3][1] * right[1][1] +
                left[3][2] * right[2][1] + left[3][3] * right[3][1];
    res[3][2] = left[3][0] * right[0][2] + left[3][1] * right[1][2] +
                left[3][2] * right[2][2] + left[3][3] * right[3][2];
    res[3][3] = left[3][0] * right[0][3] + left[3][1] * right[1][3] +
                left[3][2] * right[2][3] + left[3][3] * right[3][3];

    return res;
}

template<typename L, typename R, int W, int H>
inline Mat<R, W, H> operator+(const Mat<L, W, H> &left, const Mat<R, W, H> &right) {
    Mat<R, W, H> result;

    for (int y = 0; y < W; y++)
        for (int x = 0; x < H; x++)
            result[y][x] = left[y][x] + right[y][x];

    return result;
}

template<typename L, typename R, int W, int H>
inline Mat<R, W, H> operator-(const Mat<L, W, H> &left, const Mat<R, W, H> &right) {
    Mat<R, W, H> result;

    for (int y = 0; y < W; y++)
        for (int x = 0; x < H; x++)
            result[y][x] = left[y][x] - right[y][x];

    return result;
}

NS_END
