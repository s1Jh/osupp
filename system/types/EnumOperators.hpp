/*******************************************************************************
 * Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#pragma once

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

#include "Traits.hpp"
#include "define.hpp"

NS_BEGIN

namespace detail
{

// This is the type that will hold all the strings.
// Each enumeration type will declare its own specialization.
// Any enum that does not have a specialization will generate a compiler error
// indicating that there is no definition of this variable (as there should be
// be no definition of a generic version).
template<typename T>
struct EnumStrings
{
    static char const *data[];
};

// This is a utility type.
// Created automatically. Should not be used directly.
template<typename T>
struct EnumRefHolder
{
    T &value;

    EnumRefHolder(T &value)
        : value(value)
    {}
};

template<typename T>
struct EnumConstRefHolder
{
    T const &value;

    EnumConstRefHolder(T const &value)
        : value(value)
    {}
};

} // namespace detail

template<typename T>
detail::EnumConstRefHolder<T> EnumToString(T const &e)
{
    return detail::EnumConstRefHolder<T>(e);
}

template<typename T>
detail::EnumRefHolder<T> EnumFromString(T &e)
{
    return detail::EnumRefHolder<T>(e);
}

namespace detail
{

template<typename Enum>
struct EnumSerializable
{
    static const bool enable = false;
};

template<class Enum>
inline constexpr bool EnumSerializableV = EnumSerializable<Enum>::enable;

template<typename Enum>
requires EnumSerializableV<Enum> std::ostream &operator<<(std::ostream &os,
                                                          Enum e)
{
    return os << EnumToString(e);
}

template<typename Enum>
requires EnumSerializableV<Enum> std::istream &operator>>(std::istream &is,
                                                          Enum &e)
{
    return is >> EnumFromString(e);
}

#define MAKE_SERIALIZABLE(x)                                                   \
  template <> struct EnumSerializable<x> { static const bool enable = true; };

} // namespace detail

#define ENABLE_BITMASK_OPERATORS(x)                                            \
  template <> struct EnableBitMaskOperators<x> {                               \
    static const bool enable = true;                                           \
  };

template<typename Enum>
struct EnableBitMaskOperators
{
    static const bool enable = false;
};

template<class T>
inline constexpr bool EnableBitMaskOperatorsV =
    EnableBitMaskOperators<T>::enable;

template<typename Enum>
requires EnableBitMaskOperatorsV<Enum>
constexpr Enum operator~(Enum val)
{
    using underlying = typename std::underlying_type<Enum>::type;

    return static_cast<Enum>(~static_cast<underlying>(val));
}

template<typename Enum>
requires EnableBitMaskOperatorsV<Enum>
constexpr Enum operator&(Enum lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;

    return static_cast<Enum>(static_cast<underlying>(lhs) &
        static_cast<underlying>(rhs));
}

template<typename Enum>
requires EnableBitMaskOperatorsV<Enum>
constexpr Enum operator|(Enum lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;

    return static_cast<Enum>(static_cast<underlying>(lhs) |
        static_cast<underlying>(rhs));
}

template<typename Enum>
requires EnableBitMaskOperatorsV<Enum>
constexpr Enum operator^(Enum lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;

    return static_cast<Enum>(static_cast<underlying>(lhs) ^
        static_cast<underlying>(rhs));
}

template<typename Enum>
requires EnableBitMaskOperatorsV<Enum>
constexpr Enum operator&=(Enum &lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;

    lhs = static_cast<Enum>(static_cast<underlying>(lhs) &
        static_cast<underlying>(rhs));

    return lhs;
}

template<typename Enum>
requires EnableBitMaskOperatorsV<Enum>
constexpr Enum operator|=(Enum &lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;

    lhs = static_cast<Enum>(static_cast<underlying>(lhs) |
        static_cast<underlying>(rhs));

    return lhs;
}

template<typename Enum>
requires EnableBitMaskOperatorsV<Enum>
constexpr Enum operator^=(Enum &lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;

    lhs = static_cast<Enum>(static_cast<underlying>(lhs) ^
        static_cast<underlying>(rhs));

    return lhs;
}

NS_END
