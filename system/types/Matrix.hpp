/*******************************************************************************
 * Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#pragma once

#include "define.hpp"

#include <cstddef>
#include <type_traits>
#include <array>

namespace PROJECT_NAMESPACE
{

template<typename T, unsigned long w, unsigned long h> requires std::is_arithmetic_v<T>
class Mat
{
public:
    inline constexpr explicit Mat(T fill = 0)
    {
        for (unsigned long x = 0; x < w; x++)
            for (unsigned long y = 0; y < h; y++)
                at(x, y) = fill;
    }

    inline constexpr explicit Mat(std::array<T, w * h> data)
    {
        for (unsigned long x = 0; x < w; x++)
            for (unsigned long y = 0; y < h; y++)
                at(x, y) = data.at(y * w + x);
    }

    Mat(const Mat &other)
    {
        for (unsigned long x = 0; x < w; x++)
            for (unsigned long y = 0; y < h; y++)
                at(x, y) = other.at(x, y);
    }

    Mat(const Mat &&other) noexcept
    {
        for (unsigned long x = 0; x < w; x++)
            for (unsigned long y = 0; y < h; y++)
                at(x, y) = other.at(x, y);
    }

    inline Mat &operator=(const Mat &other)
    {
        if (this == &other)
            return *this;

        for (unsigned long x = 0; x < w; x++)
            for (unsigned long y = 0; y < h; y++)
                at(x, y) = other.at(x, y);

        return *this;
    }

    inline Mat &operator=(Mat &&other) noexcept
    {
        for (unsigned long x = 0; x < w; x++)
            for (unsigned long y = 0; y < h; y++)
                at(x, y) = other.at(x, y);

        return *this;
    }

    constexpr T& at(unsigned long r, unsigned long c) noexcept
    {
        return values[(r * w + c) % (w*h)];
    }

    constexpr const T& at(unsigned long r, unsigned long c) const noexcept
    {
        return values[(r * w + c) % (w*h)];
    }

    T *GetPtr()
    { return (T *) values; }

    const T *GetCPtr() const
    { return (T *) values; }

private:
    // [columns, rows]
    T values[w*h] = {};
};

template<typename T, size_t I> using SquareMatrix = Mat<T, I, I>;

template<typename T> using Mat1 = SquareMatrix<T, 1>;
using Mat1f = Mat1<float>;
using Mat1u = Mat1<unsigned int>;
using Mat1i = Mat1<int>;
using Mat1d = Mat1<double>;

template<typename T> using Mat1x1 = SquareMatrix<T, 1>;
using Mat1x1f = Mat1x1<float>;
using Mat1x1d = Mat1x1<double>;
using Mat1x1u = Mat1x1<unsigned int>;
using Mat1x1i = Mat1x1<int>;

template<typename T> using Mat1x2 = Mat<T, 1, 2>;
using Mat1x2f = Mat1x2<float>;
using Mat1x2d = Mat1x2<double>;
using Mat1x2u = Mat1x2<unsigned int>;
using Mat1x2i = Mat1x2<int>;

template<typename T> using Mat1x3 = Mat<T, 1, 3>;
using Mat1x3f = Mat1x3<float>;
using Mat1x3d = Mat1x3<double>;
using Mat1x3u = Mat1x3<unsigned int>;
using Mat1x3i = Mat1x3<int>;

template<typename T> using Mat1x4 = Mat<T, 1, 4>;
using Mat1x4f = Mat1x4<float>;
using Mat1x4d = Mat1x4<double>;
using Mat1x4u = Mat1x4<unsigned int>;
using Mat1x4i = Mat1x4<int>;

template<typename T> using Mat2x1 = Mat<T, 2, 1>;
using Mat2x1f = Mat2x1<float>;
using Mat2x1d = Mat2x1<double>;
using Mat2x1u = Mat2x1<unsigned int>;
using Mat2x1i = Mat2x1<int>;

template<typename T> using Mat2 = SquareMatrix<T, 2>;
template<typename T> using Mat2 = SquareMatrix<T, 2>;
using Mat2f = Mat2<float>;
using Mat2d = Mat2<double>;
using Mat2u = Mat2<unsigned int>;
using Mat2i = Mat2<int>;

template<typename T> using Mat2x2 = SquareMatrix<T, 2>;
using Mat2x2f = Mat2x2<float>;
using Mat2x2d = Mat2x2<double>;
using Mat2x2u = Mat2x2<unsigned int>;
using Mat2x2i = Mat2x2<int>;

template<typename T> using Mat2x3 = Mat<T, 2, 3>;
using Mat2x3f = Mat2x3<float>;
using Mat2x3d = Mat2x3<double>;
using Mat2x3u = Mat2x3<unsigned int>;
using Mat2x3i = Mat2x3<int>;

template<typename T> using Mat2x4 = Mat<T, 2, 4>;
using Mat2x4f = Mat2x4<float>;
using Mat2x4d = Mat2x4<double>;
using Mat2x4u = Mat2x4<unsigned int>;
using Mat2x4i = Mat2x4<int>;

template<typename T> using Mat3x1 = Mat<T, 3, 1>;
using Mat3x1f = Mat3x1<float>;
using Mat3x1d = Mat3x1<double>;
using Mat3x1u = Mat3x1<unsigned int>;
using Mat3x1i = Mat3x1<int>;

template<typename T> using Mat3x2 = Mat<T, 3, 2>;
using Mat3x2f = Mat3x2<float>;
using Mat3x2d = Mat3x2<double>;
using Mat3x2u = Mat3x2<unsigned int>;
using Mat3x2i = Mat3x2<int>;

template<typename T> using Mat3 = SquareMatrix<T, 3>;
using Mat3f = Mat3<float>;
using Mat3d = Mat3<double>;
using Mat3u = Mat3<unsigned int>;
using Mat3i = Mat3<int>;

template<typename T> using Mat3x3 = SquareMatrix<T, 3>;
using Mat3x3f = Mat3x3<float>;
using Mat3x3d = Mat3x3<double>;
using Mat3x3u = Mat3x3<unsigned int>;
using Mat3x3i = Mat3x3<int>;

template<typename T> using Mat3x4 = Mat<T, 3, 4>;
using Mat3x4f = Mat3x4<float>;
using Mat3x4d = Mat3x4<double>;
using Mat3x4u = Mat3x4<unsigned int>;
using Mat3x4i = Mat3x4<int>;

template<typename T> using Mat4x1 = Mat<T, 4, 1>;
using Mat4x1f = Mat4x1<float>;
using Mat4x1d = Mat4x1<double>;
using Mat4x1u = Mat4x1<unsigned int>;
using Mat4x1i = Mat4x1<int>;

template<typename T> using Mat4x2 = Mat<T, 4, 2>;
using Mat4x2f = Mat4x2<float>;
using Mat4x2d = Mat4x2<double>;
using Mat4x2u = Mat4x2<unsigned int>;
using Mat4x2i = Mat4x2<int>;

template<typename T> using Mat4x3 = Mat<T, 4, 3>;
using Mat4x3f = Mat4x3<float>;
using Mat4x3d = Mat4x3<double>;
using Mat4x3u = Mat4x3<unsigned int>;
using Mat4x3i = Mat4x3<int>;

template<typename T> using Mat4 = SquareMatrix<T, 4>;
using Mat4f = Mat4<float>;
using Mat4d = Mat4<double>;
using Mat4u = Mat4<unsigned int>;
using Mat4i = Mat4<int>;

template<typename T> using Mat4x4 = SquareMatrix<T, 4>;
using Mat4x4f = Mat4x4<float>;
using Mat4x4d = Mat4x4<double>;
using Mat4x4u = Mat4x4<unsigned int>;
using Mat4x4i = Mat4x4<int>;

template<typename T>
constexpr Mat2<T> MAT2_NO_TRANSFORM = Mat2<T>({1.f, 0.f, 0.f, 1.f});

template<typename T>
constexpr Mat3<T> MAT3_NO_TRANSFORM = Mat3<T>({1.f, 0.f, 0.f, 0.f, 1.f, 0.f,
                                               0.f, 0.f, 1.f});

template<typename T>
constexpr Mat4<T> MAT4_NO_TRANSFORM = Mat4<T>({1.f, 0.f, 0.f, 0.f, 0.f, 1.f,
                                               0.f, 0.f, 0.f, 0.f, 1.f, 0.f,
                                               0.f, 0.f, 0.f, 1.f});

template<typename L, int Lw, int Lh, typename R, int Rw, int Rh>
bool operator==(const Mat<L, Lw, Lh> &left, const Mat<R, Rw, Rh> &right)
{
    if (Lw != Rw || Lh != Rh)
        return false;

    for (int y = 0; y < Rw; y++)
        for (int x = 0; x < Lh; x++) {
            if (left.at(y, x) != right.at(y, x))
                return false;
        }

    return true;
}

template<typename L, int Lw, int Lh, typename R, int Rw, int Rh>
bool operator!=(const Mat<L, Lw, Lh> &left, const Mat<R, Rw, Rh> &right)
{
    if (Lw != Rw || Lh != Rh)
        return true;

    for (int y = 0; y < Rw; y++)
        for (int x = 0; x < Lh; x++) {
            if (left.at(y, x) != right.at(y, x))
                return true;
        }

    return false;
}

// for assymetric operations or otherwise not expanded operations
template<typename L, unsigned long Lw, unsigned long Lh, typename R, unsigned long Rw, unsigned long Rh>
typename std::enable_if<((Rw != Lw || Rh != Lh) || Lw > 4),
                        Mat<R, Rw, Lh>>::type
operator*(const Mat<L, Lw, Lh> &left, const Mat<R, Rw, Rh> &right)
{
    static_assert(Rh == Lw, "Invalid matrix multiplication parameters: left "
                            "width and right height don't match!");

    Mat<R, Rw, Lh> result;

    for (unsigned long c = 0; c < Rw; c++)
        for (unsigned long r = 0; r < Lh; r++) {
            R accum = 0;
            for (unsigned long z = 0; z < Rh; z++) {
                auto lhs = left.at(z, r);
                auto rhs = right.at(z, c);
                accum += lhs * rhs;
            }

            result.at(r, c) = accum;
        }

    return result;
}

template<typename L, typename R>
Mat2<L> operator*(const Mat2<L> &left, const Mat2<R> &right)
{
    Mat2<L> res;

    res.at(0, 0) = left.at(0, 0) * right.at(0, 0) + left.at(0, 1) * right.at(1, 0);
    res.at(0, 1) = left.at(0, 0) * right.at(0, 1) + left.at(0, 1) * right.at(1, 1);
    res.at(1, 0) = left.at(1, 0) * right.at(0, 1) + left.at(1, 1) * right.at(1, 0);
    res.at(1, 1) = left.at(1, 0) * right.at(0, 1) + left.at(1, 1) * right.at(1, 1);

    return res;
}

template<typename L, typename R>
Mat3<L> operator*(const Mat3<L> &left, const Mat3<R> &right)
{
    Mat3<L> res;

    res.at(0, 0) = left.at(0, 0) * right.at(0, 0) + left.at(0, 1) * right.at(1, 0) + left.at(0, 2) * right.at(2, 0);
    res.at(0, 1) = left.at(0, 0) * right.at(0, 1) + left.at(0, 1) * right.at(1, 1) + left.at(0, 2) * right.at(2, 1);
    res.at(0, 2) = left.at(0, 0) * right.at(0, 2) + left.at(0, 1) * right.at(1, 2) + left.at(0, 2) * right.at(2, 2);

    res.at(1, 0) = left.at(1, 0) * right.at(0, 0) + left.at(1, 1) * right.at(1, 0) + left.at(1, 2) * right.at(2, 0);
    res.at(1, 1) = left.at(1, 0) * right.at(0, 1) + left.at(1, 1) * right.at(1, 1) + left.at(1, 2) * right.at(2, 1);
    res.at(1, 2) = left.at(1, 0) * right.at(0, 2) + left.at(1, 1) * right.at(1, 2) + left.at(1, 2) * right.at(2, 2);

    res.at(2, 0) = left.at(2, 0) * right.at(0, 0) + left.at(2, 1) * right.at(1, 0) + left.at(2, 2) * right.at(2, 0);
    res.at(2, 1) = left.at(2, 0) * right.at(0, 1) + left.at(2, 1) * right.at(1, 1) + left.at(2, 2) * right.at(2, 1);
    res.at(2, 2) = left.at(2, 0) * right.at(0, 2) + left.at(2, 1) * right.at(1, 2) + left.at(2, 2) * right.at(2, 2);

    return res;
}

template<typename L, typename R>
Mat4<L> operator*(const Mat4<L> &left, const Mat4<R> &right)
{
    Mat4<L> res;

    res.at(0, 0) = left.at(0, 0) * right.at(0, 0) + left.at(0, 1) * right.at(1, 0) +
        left.at(0, 2) * right.at(2, 0) + left.at(0, 3) * right.at(3, 0);
    res.at(0, 1) = left.at(0, 0) * right.at(0, 1) + left.at(0, 1) * right.at(1, 1) +
        left.at(0, 2) * right.at(2, 1) + left.at(0, 3) * right.at(3, 1);
    res.at(0, 2) = left.at(0, 0) * right.at(0, 2) + left.at(0, 1) * right.at(1, 2) +
        left.at(0, 2) * right.at(2, 2) + left.at(0, 3) * right.at(3, 2);
    res.at(0, 3) = left.at(0, 0) * right.at(0, 3) + left.at(0, 1) * right.at(1, 3) +
        left.at(0, 2) * right.at(2, 3) + left.at(0, 3) * right.at(3, 3);
    res.at(1, 0) = left.at(1, 0) * right.at(0, 0) + left.at(1, 1) * right.at(1, 0) +
        left.at(1, 2) * right.at(2, 0) + left.at(1, 3) * right.at(3, 0);
    res.at(1, 1) = left.at(1, 0) * right.at(0, 1) + left.at(1, 1) * right.at(1, 1) +
        left.at(1, 2) * right.at(2, 1) + left.at(1, 3) * right.at(3, 1);
    res.at(1, 2) = left.at(1, 0) * right.at(0, 2) + left.at(1, 1) * right.at(1, 2) +
        left.at(1, 2) * right.at(2, 2) + left.at(1, 3) * right.at(3, 2);
    res.at(1, 3) = left.at(1, 0) * right.at(0, 3) + left.at(1, 1) * right.at(1, 3) +
        left.at(1, 2) * right.at(2, 3) + left.at(1, 3) * right.at(3, 3);
    res.at(2, 0) = left.at(2, 0) * right.at(0, 0) + left.at(2, 1) * right.at(1, 0) +
        left.at(2, 2) * right.at(2, 0) + left.at(2, 3) * right.at(3, 0);
    res.at(2, 1) = left.at(2, 0) * right.at(0, 1) + left.at(2, 1) * right.at(1, 1) +
        left.at(2, 2) * right.at(2, 1) + left.at(2, 3) * right.at(3, 1);
    res.at(2, 2) = left.at(2, 0) * right.at(0, 2) + left.at(2, 1) * right.at(1, 2) +
        left.at(2, 2) * right.at(2, 2) + left.at(2, 3) * right.at(3, 2);
    res.at(2, 3) = left.at(2, 0) * right.at(0, 3) + left.at(2, 1) * right.at(1, 3) +
        left.at(2, 2) * right.at(2, 3) + left.at(2, 3) * right.at(3, 3);
    res.at(3, 0) = left.at(3, 0) * right.at(0, 0) + left.at(3, 1) * right.at(1, 0) +
        left.at(3, 2) * right.at(2, 0) + left.at(3, 3) * right.at(3, 0);
    res.at(3, 1) = left.at(3, 0) * right.at(0, 1) + left.at(3, 1) * right.at(1, 1) +
        left.at(3, 2) * right.at(2, 1) + left.at(3, 3) * right.at(3, 1);
    res.at(3, 2) = left.at(3, 0) * right.at(0, 2) + left.at(3, 1) * right.at(1, 2) +
        left.at(3, 2) * right.at(2, 2) + left.at(3, 3) * right.at(3, 2);
    res.at(3, 3) = left.at(3, 0) * right.at(0, 3) + left.at(3, 1) * right.at(1, 3) +
        left.at(3, 2) * right.at(2, 3) + left.at(3, 3) * right.at(3, 3);

    return res;
}

template<typename L, typename R, int W, int H>
inline Mat<R, W, H> operator+(const Mat<L, W, H> &left,
                              const Mat<R, W, H> &right)
{
    Mat<R, W, H> result;

    for (int y = 0; y < W; y++)
        for (int x = 0; x < H; x++)
            result.at(y, x) = left.at(y, x) + right.at(y, x);

    return result;
}

template<typename L, typename R, int W, int H>
inline Mat<R, W, H> operator-(const Mat<L, W, H> &left,
                              const Mat<R, W, H> &right)
{
    Mat<R, W, H> result;

    for (int y = 0; y < W; y++)
        for (int x = 0; x < H; x++)
            result.at(y, x) = left.at(y, x) - right.at(y, x);

    return result;
}

}
