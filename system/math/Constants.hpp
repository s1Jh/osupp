// Copyright (c) 2023 sijh (s1Jh.199[at]gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//
// Created by sijh on 21.01.23.
//

#pragma once

#include "define.hpp"

#include <type_traits>

namespace PROJECT_NAMESPACE
{

namespace math
{

namespace detail
{

/// @brief The raw value of PI before being cast into a specific number type.
template<typename T> requires std::floating_point<T>
constexpr T implPI = static_cast<T>(3.141592653589793);

/// @brief The raw value of E before being cast into a specific number type.
template<typename T> requires std::floating_point<T>
constexpr T implE = static_cast<T>(2.7182818284590452);
} // namespace detail


/// @brief The value of PI in double precision floating point number.
constexpr auto PI = detail::implPI<double>;

/// @brief The value of PI in single precision floating point number.
constexpr auto fPI = detail::implPI<float>;

/// @brief The value of euler's number in double precision floating point number.
constexpr auto E = detail::implE<double>;

/// @brief The value of euler's number in single precision floating point number.
constexpr auto fE = detail::implE<float>;

}

/// @brief Multiplies the value by PI.
/// @param val The value.
/// @return Value times PI.
constexpr long double operator ""_pi(long double val)
{ return val * math::PI; }

/// @brief Multiplies the value by E.
/// @param val The value.
/// @return Value times E.
constexpr long double operator ""_e(long double val)
{ return val * math::E; }

}