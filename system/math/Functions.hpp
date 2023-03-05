//=*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*=
// Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
//                                      =*=
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//                                      =*=
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//                                      =*=
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.                            =*=
//=*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*=
#pragma once

#include "define.hpp"

#include "Vector.hpp"
#include "Color.hpp"
#include "Trigonometry.hpp"
#include "Constants.hpp"

#include <type_traits>

namespace PROJECT_NAMESPACE::math
{

	/**
	 * Calculates the parametric equation for a circle with radius rad.
	 * @tparam T Vector type.
	 * @param t Parameter.
	 * @param rad Radius of the circle.
	 * @return Position in R2.
	 */
	template <typename T>
		requires std::is_floating_point_v<T>
	vec2d<T> Circle(T t, T rad)
	{
		return {Cos(t) * rad, Sin(t) * rad};
	}

	/**
	 * Returns a color from a continuous rainbow based on parameter t.
	 * @tparam T Parameter type.
	 * @param t Parameter.
	 * @return Color of the rainbow.
	 */
	template <typename T>
		requires std::is_floating_point_v<T>
	color Rainbow(T t)
	{
		return {
			Sin(t),
			Sin(t + 0.333333_pi),
			Sin(t + 0.666666_pi),
			1.0};
	}

	// Quadratic functions where roots = { -a; +a }
	template <typename T>
		requires std::is_floating_point_v<T>
	T QuadRR(T x, T a)
	{
		return T(-std::pow(x / a, 2)) + 1.f;
	}

	// Linear function which equals to 0 at -a and 1 at +a
	template <typename T>
		requires std::is_floating_point_v<T>
	T LinearES(T x, T a)
	{
		return (x + a) / (2 * a);
	}

	// Linear function which equals to 1 at -a and 0 at +a
	template <typename T>
		requires std::is_floating_point_v<T>
	T LinearSE(T x, T a)
	{
		return 1.f - LinearES(x, a);
	}

	// Linear function which equals to 1 at 0 and 0 at -a and +a
	template <typename T>
		requires std::is_floating_point_v<T>
	T LinearUD(T x, T a)
	{
		return 1 - std::abs(x / a);
	}

	template <typename T>
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

}
