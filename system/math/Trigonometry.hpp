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

#include "Constants.hpp"

#include <type_traits>
#include <cmath>

namespace PROJECT_NAMESPACE::math
{

	template <typename T>
	constexpr T DegreeToRad(T deg)
	{
		return deg * (PI / 180.0);
	}

	template <typename T>
	constexpr T RadToDegree(T rad)
	{
		return rad * (180.0 / PI);
	}

	template <typename T>
		requires std::is_floating_point_v<T>
	T Sin(T x)
	{
		return static_cast<T>(std::sin(static_cast<double>(x)));
	}

	template <typename T>
		requires std::is_floating_point_v<T>
	T Cos(T x)
	{
		return static_cast<T>(std::cos(static_cast<double>(x)));
	}

	template <typename T>
		requires std::is_floating_point_v<T>
	T Tan(T x)
	{
		return static_cast<T>(std::tan(static_cast<double>(x)));
	}

	template <typename T>
		requires std::is_floating_point_v<T>
	T Cot(T x)
	{
		return static_cast<T>(1.0 / std::tan(static_cast<double>(x)));
	}
	template <typename T>
		requires std::is_floating_point_v<T>
	T ArcSin(T x)
	{
		return static_cast<T>(std::asin(static_cast<double>(x)));
	}

	template <typename T>
		requires std::is_floating_point_v<T>
	T ArcCos(T x)
	{
		return static_cast<T>(std::acos(static_cast<double>(x)));
	}

	template <typename T>
		requires std::is_floating_point_v<T>
	T ArcTan(T x)
	{
		return static_cast<T>(std::atan(static_cast<double>(x)));
	}

	template <typename T>
		requires std::is_floating_point_v<T>
	T ArcCot(T x)
	{
		return static_cast<T>(1.0 / std::atan(static_cast<double>(x)));
	}

}
