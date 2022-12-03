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

#include "Rect.hpp"
#include "Vec2.hpp"
#include "Line.hpp"
#include "Exponential.hpp"

#include <optional>

NS_BEGIN

namespace math
{

template<typename T1, typename T2>
T1 Pyth(T1 a, T2 b)
{
	return Sqrt(Pow(a, 2) + Pow(b, 2));
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

template<typename LineT, typename PointT>
double GetSide(line<LineT> P, vec2d<PointT> A)
{
	return (P.B.x - P.A.x) * (A.y - P.A.y) - (P.B.y - P.A.y) * (A.x - P.A.x);
}

template<typename T1, typename T2>
std::optional<vec2d<T1>> GetIntersect(line<T1> L1, line<T2> L2)
{
	auto det = [](double a, double b, double c, double d) -> double
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

}

NS_END
