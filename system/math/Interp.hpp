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

namespace PROJECT_NAMESPACE::math
{

	template <typename A, typename B, typename X>
	X Lerp(A a, B b, X x)
	{
		return (1 - x) * a + x * b;
	}

	template <typename T>
	T BiLerp(T a, T b, T c, T d, T x, T y)
	{
		T i0 = Lerp(a, b, x);
		T i1 = Lerp(c, d, x);

		T value = Lerp(i0, i1, y);
		return value;
	}

	template <typename T1, typename T2, typename T3>
		requires std::floating_point<T1> and std::floating_point<T2> and
				 std::floating_point<T3>
	vec2d<T1> BiLerp(vec2d<T1> a, vec2d<T2> b, T3 x)
	{
		vec2d<T1> v;

		v[0] = Lerp(a[0], b[0], x);
		v[1] = Lerp(a[1], b[1], x);

		return v;
	}

}
