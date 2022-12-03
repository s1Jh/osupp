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

#include "Types.hpp"
#include "Random.hpp"
#include "MatrixMath.hpp"
#include "Functions.hpp"
#include "Interp.hpp"
#include "Exponential.hpp"
#include "Trigonometry.hpp"
#include "Geometry.hpp"
#include "Range.hpp"
#include "VectorMath.hpp"

#include <optional>

NS_BEGIN

namespace math
{

namespace detail
{
// 15 spaces was good enough for NASA, good enough for us
template<typename T> requires std::floating_point<T>
constexpr T implPI = static_cast<T>(3.141592653589793);

template<typename T> requires std::floating_point<T>
constexpr T implE = static_cast<T>(2.7182818284590452);
} // namespace detail

constexpr auto PI = detail::implPI<double>;

constexpr auto fPI = detail::implPI<float>;

constexpr auto E = detail::implE<double>;

constexpr auto fE = detail::implE<float>;

constexpr double SIN45 = 0.707106781;

template<typename T>
constexpr T DegreeToRad(T deg)
{ return deg * (PI / 180.0); }

template<typename T>
constexpr T RadToDegree(T rad)
{ return rad * (180.0 / PI); }

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
{ return a < (T1)b ? a : (T1)b; }

template<typename T1, typename T2>
requires std::is_arithmetic_v<T1> and std::is_arithmetic_v<T2>
constexpr T1 Max(T1 a, T2 b)
{ return a > (T1)b ? a : (T1)b; }

template<typename XT, typename MinT, typename MaxT>
XT Clamp(XT x, MinT min, MaxT max)
{
	auto realMin = math::Min(min, max);
	auto realMax = math::Max(min, max);

	x = Max(x, realMin);
	x = Min(x, realMax);
	return x;
}

}

constexpr long double operator ""_pi(long double val)
{ return val * math::PI; }

constexpr long double operator ""_e(long double val)
{ return val * math::E; }

NS_END
