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

#include "Types.hpp"

#include "Constraint.hpp"
#include "Util.hpp"

namespace PROJECT_NAMESPACE
{
/**
 * Converts a string representation of color into color.
 * @param in String repr.
 * @return Color.
 */
template<>
std::optional<color> FromString<color>(const std::string &in)
{
    color8 truncated;
    auto components = GetCharacterSeparatedValues(in, ',');
    truncated.r = math::Min(GetParam<int>(components, 0, 0), 255);
    truncated.g = math::Min(GetParam<int>(components, 1, 0), 255);
    truncated.b = math::Min(GetParam<int>(components, 2, 0), 255);
    truncated.a = math::Min(GetParam<int>(components, 3, 0), 255);
    return (color) truncated;
}

/**
 * Converts a color to string representation.
 * @param in Color.
 * @return String repr.
 */
template<>
std::string ToString<color>(const color &in)
{
    auto in8 = (color8) in;
    return std::to_string(in8.r) + ','
         + std::to_string(in8.g) + ','
         + std::to_string(in8.b) + ','
         + std::to_string(in8.a);
}

/**
 * Converts a string representation of color into color.
 * @param in String repr.
 * @return Color.
 */
template<>
std::optional<color8> FromString<color8>(const std::string &in)
{
    color8 truncated;
    auto components = GetCharacterSeparatedValues(in, ',');
    truncated.r = math::Min(GetParam<int>(components, 0, 0), 255);
    truncated.g = math::Min(GetParam<int>(components, 1, 0), 255);
    truncated.b = math::Min(GetParam<int>(components, 2, 0), 255);
    truncated.a = math::Min(GetParam<int>(components, 3, 0), 255);
    return truncated;
}

/**
 * Converts a color to string representation.
 * @param in Color.
 * @return String repr.
 */
template<>
std::string ToString<color8>(const color8 &in)
{
    return std::to_string(in.r) + ','
        + std::to_string(in.g) + ','
        + std::to_string(in.b) + ','
        + std::to_string(in.a);
}

}
