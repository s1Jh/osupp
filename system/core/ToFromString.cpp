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

#include "ToFromString.hpp"
#include "Util.hpp"
#include "Constraint.hpp"

namespace PROJECT_NAMESPACE {

/**
 * Opaque specialization for completeness sake.
 * @param in Any string.
 * @return The input string.
 */
template<>
std::string ToString<std::string>(const std::string &in)
{
    return in;
}

/**
 * Convert bool to string.
 * @param in Value.
 * @return String representation.
 */
template<>
std::string ToString<bool>(const bool &in)
{
    return in ? "true" : "false";
}

/**
 * Extracts a bool value from string.
 * @param in String representation.
 * @return Converted bool.
 */
template<>
std::optional<bool> FromString<bool>(const std::string &in)
{
    static const char* TRUE_VALUES[] = {"true", "yes", "1", "on"};
    static const char* FALSE_VALUES[] = {"false", "no", "0", "off"};
    constexpr size_t CEIL = math::Min(ARRAY_SIZE(TRUE_VALUES), ARRAY_SIZE(FALSE_VALUES));

    auto trimmed = TrimCopy(in);
    LowerCase(trimmed);

    for (size_t i = 0; i < CEIL; i++) {
        if (trimmed == TRUE_VALUES[i]) {
            return true;
        } else if (trimmed == FALSE_VALUES[i]) {
            return false;
        }
    }

    return {};
}

/**
 * Opaque specialization for completeness sake.
 * @param in String value.
 * @return Input string value.
 */
template<>
std::optional<std::string> FromString<std::string>(const std::string &in)
{
    return in;
}

}
