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

#include "Vec4.hpp"
#include "Exponential.hpp"

NS_BEGIN

namespace math
{

template<typename T>
inline T Mag(const vec4d<T> &v)
{
	return Sqrt(Pow(v.x, 2) + Pow(v.y, 2) + Pow(v.z, 2) + Pow(v.w, 2));
}

template<typename T>
inline T Mag2(const vec4d<T> &v)
{
	return Pow(v.x, 2) + Pow(v.y, 2) + Pow(v.z, 2) + Pow(v.w, 2);
}

template<typename T>
inline vec4d<T> Normalize(const vec4d<T> &v)
{
	return v / Mag(v);
}

template<typename T1>
inline vec4d<T1> Negate(const vec4d<T1> &A)
{
	return vec4d<T1>{-A.x, -A.y, -A.z, -A.w};
}

template<typename T1, typename T2>
inline T1 Distance(const vec4d<T1> &a, const vec4d<T2> &b)
{
	return Mag(a - b);
}

}

NS_END
