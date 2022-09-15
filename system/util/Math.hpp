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

#include "Types.hpp"
#include "define.hpp"

#include <optional>

NS_BEGIN

namespace detail
{
// 15 spaces was good enough for NASA, good enough for us
template<typename T> requires std::floating_point<T>
constexpr T implPI = 3.141592653589793;

template<typename T> requires std::floating_point<T>
constexpr T implE = 2.7182818284590452;
} // namespace detail

constexpr auto PI = detail::implPI<double>;

constexpr auto fPI = detail::implPI<float>;

constexpr auto E = detail::implE<double>;

constexpr auto fE = detail::implE<float>;

constexpr double SIN45 = 0.707106781;

constexpr long double operator ""_pi(long double val)
{ return val * PI; }

constexpr long double operator ""_e(long double val)
{ return val * E; }

// Quadratic functions where roots = { -a; +a }
float QuadRR(float x, float a);

// Linear function which equals to 0 at -a and 1 at +a
float LinearES(float x, float a);

// Linear function which equals to 1 at -a and 0 at +a
float LinearSE(float x, float a);

// Linear function which equals to 1 at 0 and 0 at -a and +a
float LinearUD(float x, float a);

double Perlin2D(double x, double y, unsigned int octaves, double persistence,
                int seed, double amplitude = 1.0);

double BiLerp(double a, double b, double c, double d, double x, double y);

template<typename T>
constexpr T DegreeToRad(T deg)
{ return deg * (PI / 180.0); }

template<typename T>
constexpr T RadToDegree(T rad)
{ return rad * (180.0 / PI); }

template<typename A, typename B, typename X>
X Lerp(A a, B b, X x)
{
    return (1 - x) * a + x * b;
}

template<typename T>
constexpr T Abs(T in)
{ return in >= 0 ? in : in * -1; }

template<typename T>
requires std::is_arithmetic_v<T>
constexpr bool Sign(T in)
{ return in >= 0; }

template<typename T1, typename T2>
requires std::is_arithmetic_v<T1> and std::is_arithmetic_v<T2>
constexpr T1 Min(T1 a, T2 b)
{ return a < (T1) b ? a : (T1) b; }

template<typename T1, typename T2>
requires std::is_arithmetic_v<T1> and std::is_arithmetic_v<T2>
constexpr T1 Max(T1 a, T2 b)
{ return a > (T1) b ? a : (T1) b; }

template<typename T>
requires std::is_arithmetic_v<T>
constexpr vec2d<T> Abs(vec2d<T> in)
{ return Mag(in) >= 0 ? in : in * -1; }

template<typename T1, typename T2, typename T3>
requires std::floating_point<T1> and std::floating_point<T2> and
    std::floating_point<T3>
vec2d<T1> BiLerp(vec2d<T1> a, vec2d<T2> b, T3 x)
{
    vec2d<T1> v;

    v.x = Lerp(a.x, b.x, x);
    v.y = Lerp(a.y, b.y, x);

    return v;
}

template<typename T>
requires std::floating_point<T>
T SmoothStep(T x, T min = 0, T max = 1)
{
    if (x < min)
        return 0;

    if (x >= max)
        return 1;

    // Scale/bias into [0..1] range
    x = (x - min) / (max - min);

    return x * x * (3.0 - 2.0 * x);
}

template<typename T, size_t I>
T LaplaceExpansion(const SquareMatrix<T, I> &in)
{
    T sum = 0;
    for (int i = I; i > 0; i--)
        for (int j = I; j > 0; j--) {
            SquareMatrix<T, I - 1> lower_order;

            sum += std::pow(-1, i + j) * in[i][j] * LaplaceExpansion(lower_order);
        }
}

template<typename XT, typename MaxT, typename MinT>
bool InRangeII(XT x, MaxT a, MinT b)
{
	if (b > a)
		return (x >= a) && (x <= b);
	else
		return (x >= b) && (x <= a);
}

template<typename XT, typename MaxT, typename MinT>
bool InRangeEE(XT x, MaxT a, MinT b)
{
	if (b > a)
		return (x > a) && (x < b);
	else
		return (x > b) && (x < a);
}

template<typename XT, typename MaxT, typename MinT>
bool InRangeIE(XT x, MaxT a, MinT b)
{
	if (b > a)
		return (x >= a) && (x < b);
	else
		return (x >= b) && (x < a);
}

template<typename XT, typename MaxT, typename MinT>
bool InRangeEI(XT x, MaxT a, MinT b)
{
	if (b > a)
		return (x > a) && (x <= b);
	else
		return (x > b) && (x <= a);
}

template<typename XT, typename MaxT, typename MinT>
bool InRange(XT x, MaxT a, MinT b)
{
	return InRangeII(x, a, b);
}

template<typename RT, typename PT>
bool Intersects(rect<RT> rect, vec2d<PT> p)
{
    return (InRange(p.x, rect.position.x, rect.position.x + rect.size.w) &&
        InRange(p.y, rect.position.y, rect.position.y + rect.size.h));
}

template<typename R1, typename R2>
bool Intersects(rect<R1> left, rect <R2> right)
{
    return ((InRange(left.position.x, right.position.x,
                     right.position.x + right.size.w) ||
        InRange(left.position.x + left.size.w, right.position.x,
                right.position.x + right.size.w)) &&
        (InRange(left.position.y, right.position.y,
                 right.position.y + right.size.h) ||
            InRange(left.position.y + left.size.h, right.position.y,
                    right.position.y + right.size.h)));
}

template<typename XT, typename MinT, typename MaxT>
XT Clamp(XT x, MinT min, MaxT max)
{
	auto realMin = Min(min, max);
	auto realMax = Max(min, max);

    x = Max(x, realMin);
    x = Min(x, realMax);
    return x;
}

template<typename LineT, typename PointT>
double GetSide(line<LineT> P, vec2d<PointT> A)
{
    return (P.B.x - P.A.x) * (A.y - P.A.y) - (P.B.y - P.A.y) * (A.x - P.A.x);
}

template<typename T1, typename T2>
double Cross(vec2d<T1> a, vec2d<T2> b)
{
    return a.x * b.y - b.x * a.y;
}

template<typename T>
vec2d<T> Normal(vec2d<T> vec)
{ return {vec.y, -vec.x}; }

template<typename T1, typename T2>
T1 Pyth(T1 a, T2 b)
{
    return std::sqrt(std::pow(a, 2) + std::pow(b, 2));
}

template<typename T1, typename T2>
std::optional<vec2d<T1>> GetIntersect(line<T1> L1, line<T2> L2)
{
    auto det = [](double a, double b, double c, double d) -> float
    {
        return a * d - b * c;
    };

    float x1 = L1.A.x;
    float x2 = L1.B.x;
    float x3 = L2.A.x;
    float x4 = L2.B.x;

    float y1 = L1.A.y;
    float y2 = L1.B.y;
    float y3 = L2.A.y;
    float y4 = L2.B.y;

    double det1 = det(x1, y1, x2, y2);
    double det2 = det(x3, y3, x4, y4);
    double x1mx2 = x1 - x2;
    double x3mx4 = x3 - x4;
    double y1my2 = y1 - y2;
    double y3my4 = y3 - y4;

    double xnom = det(det1, x1mx2, det2, x3mx4);
    double ynom = det(det1, y1my2, det2, y3my4);
    double denom = det(x1mx2, y1my2, x3mx4, y3my4);
    if (denom == 0.0) // Lines don't seem to cross
    {
        return {};
    }

    double ixOut = xnom / denom;
    double iyOut = ynom / denom;

    return vec2d<T1>{ixOut, iyOut};
}

/**********************
** MATRIX OPERATIONS **
**********************/

template<typename T>
Mat4<T> CreateProjection(float fov, float aspect_ratio, float nearz,
                         float farz)
{
    float scale = std::tan(fov * 0.5f) * nearz;
    float r = aspect_ratio * scale;
    float l = -r;
    float t = scale;
    float b = -t;

    Mat4<T> mat;
    mat.row_view(0)[0] = 2 * nearz / (r - l);
    mat.row_view(0)[1] = 0;
    mat.row_view(0)[2] = 0;
    mat.row_view(0)[3] = 0;

    mat.row_view(1)[0] = 0;
    mat.row_view(1)[1] = 2 * nearz / (t - b);
    mat.row_view(1)[2] = 0;
    mat.row_view(1)[3] = 0;

    mat.row_view(2)[0] = (r + l) / (r - l);
    mat.row_view(2)[1] = (t + b) / (t - b);
    mat.row_view(2)[2] = -(farz + nearz) / (farz - nearz);
    mat.row_view(2)[3] = -1;

    mat.row_view(3)[0] = 0;
    mat.row_view(3)[1] = 0;
    mat.row_view(3)[2] = -2 * farz * nearz / (farz - nearz);
    mat.row_view(3)[3] = 0;

    return mat;
}

template<typename T1, typename T2>
Mat4<T1> LookAt(const vec3d<T2> &eye, const vec3d<T2> &target,
                const vec3d<T2> &up)
{
    vec3d<T2> forward = Normalize(target - eye);
    vec3d<T2> left = Normalize(Cross(forward, up));
    vec3d<T2> upp = Cross(left, forward);

    Mat4<T1> mat;

    mat.column_view(0)[0] = left.x;
    mat.column_view(0)[1] = left.y;
    mat.column_view(0)[2] = left.z;
    mat.column_view(0)[3] = -Dot(left, eye);

    mat.column_view(1)[0] = upp.x;
    mat.column_view(1)[1] = upp.y;
    mat.column_view(1)[2] = upp.z;
    mat.column_view(1)[3] = -Dot(upp, eye);

    mat.column_view(2)[0] = forward.x;
    mat.column_view(2)[1] = forward.y;
    mat.column_view(2)[2] = forward.z;
    mat.column_view(2)[3] = -Dot(forward, eye);

    mat.column_view(3)[0] = 0;
    mat.column_view(3)[1] = 0;
    mat.column_view(3)[2] = 0;
    mat.column_view(3)[3] = 1;

    return mat;
}

template<typename T>
T Det(const Mat2<T> &mat)
{
    return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
}

template<typename T>
T Det(const Mat3<T> &mat)
{
    return mat[0][0] * mat[1][1] * mat[2][2] + mat[1][0] * mat[2][1] * mat[0][2] +
        mat[2][0] * mat[0][1] * mat[1][2] - mat[0][0] * mat[2][1] * mat[1][2] -
        mat[1][0] * mat[0][1] * mat[2][2] - mat[2][0] * mat[1][1] * mat[0][2];
}

template<typename T>
T Det(const Mat4<T> &mat)
{
    T s0 = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
    T s1 = mat[0][0] * mat[2][1] - mat[0][1] * mat[2][0];
    T s2 = mat[0][0] * mat[3][1] - mat[0][1] * mat[3][0];
    T s3 = mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0];
    T s4 = mat[1][0] * mat[3][1] - mat[1][1] * mat[3][0];
    T s5 = mat[2][0] * mat[3][1] - mat[2][1] * mat[3][0];

    T c5 = mat[2][2] * mat[3][3] - mat[2][3] * mat[3][2];
    T c4 = mat[1][2] * mat[3][3] - mat[1][3] * mat[3][2];
    T c3 = mat[1][2] * mat[2][3] - mat[1][3] * mat[2][2];
    T c2 = mat[0][2] * mat[3][3] - mat[0][3] * mat[3][2];
    T c1 = mat[0][2] * mat[2][3] - mat[0][3] * mat[2][2];
    T c0 = mat[0][2] * mat[1][3] - mat[0][3] * mat[1][2];

    return (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);
}

template<typename T>
Mat2<T> Inverse(const Mat2<T> &mat)
{
    Mat2<T> inv;

    T invdet = 1.0 / Det<T>(mat);

    inv[0][0] = mat[1][1] * invdet;
    inv[0][1] = mat[0][1] * -invdet;
    inv[1][0] = mat[1][0] * -invdet;
    inv[1][1] = mat[0][0] * invdet;

    return inv;
}

template<typename T>
Mat3<T> Inverse(const Mat3<T> &mat)
{
    Mat3<T> inv;

    T invdet = 1.0 / Det<T>(mat);

    inv[0][0] = (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) * invdet;
    inv[0][1] = (mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2]) * invdet;
    inv[0][2] = (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]) * invdet;
    inv[1][0] = (mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2]) * invdet;
    inv[1][1] = (mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0]) * invdet;
    inv[1][2] = (mat[0][2] * mat[1][0] - mat[0][0] * mat[1][2]) * invdet;
    inv[2][0] = (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]) * invdet;
    inv[2][1] = (mat[0][1] * mat[2][0] - mat[0][0] * mat[2][1]) * invdet;
    inv[2][2] = (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]) * invdet;

    return inv;
}

template<typename T>
Mat4<T> Inverse(const Mat4<T> &mat)
{
    Mat4<T> inv;

    T invdet = 1.0 / Det<T>(mat);

    inv[0][0] =
        (mat[1][2] * mat[2][3] * mat[3][1] - mat[1][3] * mat[2][2] * mat[3][1] +
            mat[1][3] * mat[2][1] * mat[3][2] - mat[1][1] * mat[2][3] * mat[3][2] -
            mat[1][2] * mat[2][1] * mat[3][3] + mat[1][1] * mat[2][2] * mat[3][3]) *
            invdet;
    inv[0][1] =
        (mat[0][3] * mat[2][2] * mat[3][1] - mat[0][2] * mat[2][3] * mat[3][1] -
            mat[0][3] * mat[2][1] * mat[3][2] + mat[0][1] * mat[2][3] * mat[3][2] +
            mat[0][2] * mat[2][1] * mat[3][3] - mat[0][1] * mat[2][2] * mat[3][3]) *
            invdet;
    inv[0][2] =
        (mat[0][2] * mat[1][3] * mat[3][1] - mat[0][3] * mat[1][2] * mat[3][1] +
            mat[0][3] * mat[1][1] * mat[3][2] - mat[0][1] * mat[1][3] * mat[3][2] -
            mat[0][2] * mat[1][1] * mat[3][3] + mat[0][1] * mat[1][2] * mat[3][3]) *
            invdet;
    inv[0][3] =
        (mat[0][3] * mat[1][2] * mat[2][1] - mat[0][2] * mat[1][3] * mat[2][1] -
            mat[0][3] * mat[1][1] * mat[2][2] + mat[0][1] * mat[1][3] * mat[2][2] +
            mat[0][2] * mat[1][1] * mat[2][3] - mat[0][1] * mat[1][2] * mat[2][3]) *
            invdet;
    inv[1][0] =
        (mat[1][3] * mat[2][2] * mat[3][0] - mat[1][2] * mat[2][3] * mat[3][0] -
            mat[1][3] * mat[2][0] * mat[3][2] + mat[1][0] * mat[2][3] * mat[3][2] +
            mat[1][2] * mat[2][0] * mat[3][3] - mat[1][0] * mat[2][2] * mat[3][3]) *
            invdet;
    inv[1][1] =
        (mat[0][2] * mat[2][3] * mat[3][0] - mat[0][3] * mat[2][2] * mat[3][0] +
            mat[0][3] * mat[2][0] * mat[3][2] - mat[0][0] * mat[2][3] * mat[3][2] -
            mat[0][2] * mat[2][0] * mat[3][3] + mat[0][0] * mat[2][2] * mat[3][3]) *
            invdet;
    inv[1][2] =
        (mat[0][3] * mat[1][2] * mat[3][0] - mat[0][2] * mat[1][3] * mat[3][0] -
            mat[0][3] * mat[1][0] * mat[3][2] + mat[0][0] * mat[1][3] * mat[3][2] +
            mat[0][2] * mat[1][0] * mat[3][3] - mat[0][0] * mat[1][2] * mat[3][3]) *
            invdet;
    inv[1][3] =
        (mat[0][2] * mat[1][3] * mat[2][0] - mat[0][3] * mat[1][2] * mat[2][0] +
            mat[0][3] * mat[1][0] * mat[2][2] - mat[0][0] * mat[1][3] * mat[2][2] -
            mat[0][2] * mat[1][0] * mat[2][3] + mat[0][0] * mat[1][2] * mat[2][3]) *
            invdet;
    inv[2][0] =
        (mat[1][1] * mat[2][3] * mat[3][0] - mat[1][3] * mat[2][1] * mat[3][0] +
            mat[1][3] * mat[2][0] * mat[3][1] - mat[1][0] * mat[2][3] * mat[3][1] -
            mat[1][1] * mat[2][0] * mat[3][3] + mat[1][0] * mat[2][1] * mat[3][3]) *
            invdet;
    inv[2][1] =
        (mat[0][3] * mat[2][1] * mat[3][0] - mat[0][1] * mat[2][3] * mat[3][0] -
            mat[0][3] * mat[2][0] * mat[3][1] + mat[0][0] * mat[2][3] * mat[3][1] +
            mat[0][1] * mat[2][0] * mat[3][3] - mat[0][0] * mat[2][1] * mat[3][3]) *
            invdet;
    inv[2][2] =
        (mat[0][1] * mat[1][3] * mat[3][0] - mat[0][3] * mat[1][1] * mat[3][0] +
            mat[0][3] * mat[1][0] * mat[3][1] - mat[0][0] * mat[1][3] * mat[3][1] -
            mat[0][1] * mat[1][0] * mat[3][3] + mat[0][0] * mat[1][1] * mat[3][3]) *
            invdet;
    inv[2][3] =
        (mat[0][3] * mat[1][1] * mat[2][0] - mat[0][1] * mat[1][3] * mat[2][0] -
            mat[0][3] * mat[1][0] * mat[2][1] + mat[0][0] * mat[1][3] * mat[2][1] +
            mat[0][1] * mat[1][0] * mat[2][3] - mat[0][0] * mat[1][1] * mat[2][3]) *
            invdet;
    inv[3][0] =
        (mat[1][2] * mat[2][1] * mat[3][0] - mat[1][1] * mat[2][2] * mat[3][0] -
            mat[1][2] * mat[2][0] * mat[3][1] + mat[1][0] * mat[2][2] * mat[3][1] +
            mat[1][1] * mat[2][0] * mat[3][2] - mat[1][0] * mat[2][1] * mat[3][2]) *
            invdet;
    inv[3][1] =
        (mat[0][1] * mat[2][2] * mat[3][0] - mat[0][2] * mat[2][1] * mat[3][0] +
            mat[0][2] * mat[2][0] * mat[3][1] - mat[0][0] * mat[2][2] * mat[3][1] -
            mat[0][1] * mat[2][0] * mat[3][2] + mat[0][0] * mat[2][1] * mat[3][2]) *
            invdet;
    inv[3][2] =
        (mat[0][2] * mat[1][1] * mat[3][0] - mat[0][1] * mat[1][2] * mat[3][0] -
            mat[0][2] * mat[1][0] * mat[3][1] + mat[0][0] * mat[1][2] * mat[3][1] +
            mat[0][1] * mat[1][0] * mat[3][2] - mat[0][0] * mat[1][1] * mat[3][2]) *
            invdet;
    inv[3][3] =
        (mat[0][1] * mat[1][2] * mat[2][0] - mat[0][2] * mat[1][1] * mat[2][0] +
            mat[0][2] * mat[1][0] * mat[2][1] - mat[0][0] * mat[1][2] * mat[2][1] -
            mat[0][1] * mat[1][0] * mat[2][2] + mat[0][0] * mat[1][1] * mat[2][2]) *
            invdet;

    return inv;
}

template<typename T>
inline Mat3<T> MakeTranslationMatrix(const vec2d<T> &vec)
{
    return Mat3<T>({1.f, 0.f, vec.x, 0.f, 1.f, vec.y, 0.f, 0.f, 1.f});
}

template<typename T>
inline Mat3<T> MakeShearMatrix(const vec2d<T> &shear)
{
    return Mat3<T>({1.f, std::tan(shear.x), 0.f, std::tan(shear.y), 1.f, 0.f, 0.f,
                    0.f, 1.f});
}

template<typename T>
inline Mat3<T> MakeRotationMatrix(float rotation,
                                  fvec2d center = {0.0f, 0.0f})
{
    auto rotMatrix =
        Mat3<T>({std::cos(rotation), -std::sin(rotation), 0.f, std::sin(rotation),
                 std::cos(rotation), 0.f, 0.f, 0.f, 1.f});
    return MakeTranslationMatrix(center * -1) * rotMatrix *
        MakeTranslationMatrix(center);
}

template<typename T>
inline Mat3<T> MakeScaleMatrix(const vec2d<T> &scale,
							   fvec2d center = {0.0f, 0.0f})
{
    auto scaleMat = Mat3<T>({scale.x, 0.f, 0.f, 0.f, scale.y, 0.f, 0.f, 0.f, 1.f});

	return MakeTranslationMatrix(center * -1) * scaleMat *
		MakeTranslationMatrix(center);
}

/**********************
** VECTOR OPERATIONS **
**********************/
template<typename T>
inline T Mag(const vec2d<T> &v)
{
	return std::sqrt(std::pow(v.x, 2) + std::pow(v.y, 2));
}

template<typename T>
inline T Mag(const vec3d<T> &v)
{
    return std::sqrt(std::pow(v.x, 2) + std::pow(v.y, 2) + std::pow(v.z, 2));
}

template<typename T>
inline T Mag2(const vec2d<T> &v)
{
    return std::pow(v.x, 2) + std::pow(v.y, 2);
}

template<typename T1, typename T2>
inline T1 Distance(const vec2d<T1> &a, const vec2d<T2> &b)
{
    return Mag(a - b);
}

template<typename T>
inline T Mag2(const vec3d<T> &v)
{
    return std::pow(v.x, 2) + std::pow(v.y, 2) + std::pow(v.z, 2);
}

template<typename T>
inline vec3d<T> Normalize(const vec3d<T> &v)
{
    return v / Mag(v);
}

template<typename T>
inline vec3d<T> NormalizeComponents(const vec3d<T> &v)
{
    return vec3d<T>{Normalize(v.x), Normalize(v.y), Normalize(v.z)};
}

template<typename T>
inline vec2d<T> Normalize(const vec2d<T> &v)
{
    return v / Mag(v);
}

template<typename T>
inline vec2d<T> NormalizeComponents(const vec2d<T> &v)
{
    return vec3d<T>{Normalize(v.x), Normalize(v.y)};
}

template<typename T>
inline vec4d<T> Normalize(const vec4d<T> &v)
{
    return v / Mag(v);
}

template<typename T>
inline vec4d<T> NormalizeComponents(const vec4d<T> &v)
{
    return vec4d<T>{Normalize(v.x), Normalize(v.y), Normalize(v.z),
                    Normalize(v.w)};
}

template<typename T1, typename T2>
inline vec3d<T1> Cross(const vec3d<T1> &A, const vec3d<T2> &B)
{
    return vec3d<T1>{A.y * (T1) B.z - A.z * (T1) B.y, A.z * (T1) B.x - A.x * (T1) B.z,
                     A.x * (T1) B.y - A.y * (T1) B.x};
}

template<typename T1>
inline vec3d<T1> Negate(const vec3d<T1> &A)
{
    return vec3d<T1>{-A.x, -A.y, -A.z};
}

template<typename T1, typename T2>
inline T1 Dot(const vec3d<T1> &A, const vec3d<T2> &B)
{
    return A.x * B.x + A.y * B.y + A.z * B.z;
}

template<typename T>
inline vec2d<T> Rotate(vec2d<T> vec, double angle)
{
    vec2d<T> rotated;

    rotated.x = vec.x * std::cos(angle) - vec.y * std::sin(angle);
    rotated.y = vec.x * std::sin(angle) + vec.y * std::cos(angle);

    return rotated;
}

NS_END
