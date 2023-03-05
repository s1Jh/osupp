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

#pragma once

#include "define.hpp"

#include <string>
#include <ostream>
#include <optional>

#include "nameof.hpp"

#include "EnumOperators.hpp"

namespace PROJECT_NAMESPACE {

/**
 * Converts any in parameter T to string, returns the typename.
 * @tparam T The type to convert.
 * @param in The value itself.
 * @return Name of T.
 */
template<typename T>
std::string ToString(const T &in)
{
    static auto tn = NAMEOF_TYPE(decltype(in));
    return std::string(tn);
}

/**
 * Converts any bit flag enum class to string.
 * @tparam T The enum type.
 * @param in The value itself.
 * @return Set flags.
 */
template<typename T>
requires std::is_enum_v<T> && EnableBitMaskOperatorsV<T>
std::string ToString(const T &in)
{
    return std::string(NAMEOF_ENUM_FLAG(in));
}

/**
 * Converts anything that is already implicitly convertible to string.
 * @tparam T Any T convertible to string.
 * @param in The value itself.
 * @return String representation.
 */
template<typename T>
requires std::is_convertible_v<T, std::string>
std::string ToString(const T &in)
{
    return (std::string)in;
}

/**
 * Converts any enum class to string.
 * @tparam T The enum type.
 * @param in The value itself.
 * @return Enum value name.
 */
template<typename T>
requires std::is_enum_v<T> && (!EnableBitMaskOperatorsV<T>)
std::string ToString(const T &in)
{
    return std::string(NAMEOF_ENUM(in));
}

/**
 * Converts any number to string.
 * @tparam T Any numeric type.
 * @param in The value itself.
 * @return String representation.
 */
template<typename T>
requires std::is_arithmetic_v<T>
std::string ToString(const T &in)
{
    return std::to_string(in);
}

/**
 * Opaque specialization for completeness sake.
 * @param in Any string.
 * @return The input string.
 */
template<>
std::string ToString<std::string>(const std::string &in);

/**
 * Convert bool to string.
 * @param in Value.
 * @return String representation.
 */
template<>
std::string ToString<bool>(const bool &in);

/**
 * Base function template for converting string representation of a value.
 * @tparam T The type.
 * @param in The value.
 * @return Converted.
 */
template<typename T>
std::optional<T> FromString(const std::string &in)
{
    WRAP_CONSTEXPR_ASSERTION("Parse not implemented for this T");
    return {};
}

/**
 * Extracts an number from string.
 * @param in String representation.
 * @return Converted number.
 */
template<typename T>
requires std::is_arithmetic_v<T>
std::optional<T> FromString(const std::string &in)
{
    char *end;
    T val;

    if constexpr (std::is_floating_point_v<T>) {
        val = static_cast<T>(std::strtod(in.c_str(), &end));
    } else {
        val = static_cast<T>(std::strtol(in.c_str(), &end, 10));
    }

    if (end == in.c_str()) {
        return {};
    }

    return val;
}

/**
 * Extracts a bool value from string.
 * @param in String representation.
 * @return Converted bool.
 */
template<>
std::optional<bool> FromString<bool>(const std::string &in);

/**
 * Opaque specialization for completeness sake.
 * @param in String value.
 * @return Input string value.
 */
template<>
std::optional<std::string> FromString<std::string>(const std::string &in);

}
