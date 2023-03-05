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

#include <type_traits>
#include <cmath>

namespace PROJECT_NAMESPACE::math
{

template<typename T1, typename T2>
requires std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2>
T1 Pow(T1 x, T2 exponent)
{
	return static_cast<T1>(std::pow(static_cast<double>(x), static_cast<double>(exponent)));
}

template<typename T1>
requires std::is_arithmetic_v<T1>
T1 Sqrt(T1 x)
{
	return static_cast<T1>(std::sqrt(x));
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2>
T1 Sqrt(T1 x, T2 root)
{
	return static_cast<T1>(std::pow(static_cast<double>(x), 1.0 / static_cast<double>(root)));
}

}
