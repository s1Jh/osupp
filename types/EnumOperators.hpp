#pragma once

#include <type_traits>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

#include "Traits.hpp"
#include "define.hpp"

namespace GAME_TITLE {

    namespace detail {

// This is the type that will hold all the strings.
// Each enumeration type will declare its own specialization.
// Any enum that does not have a specialization will generate a compiler error
// indicating that there is no definition of this variable (as there should be
// be no definition of a generic version).
        template<typename T>
        struct EnumStrings {
            static char const *data[];
        };

// This is a utility type.
// Created automatically. Should not be used directly.
        template<typename T>
        struct EnumRefHolder {
            T &value;

            EnumRefHolder(T &value) : value(value) {}
        };

        template<typename T>
        struct EnumConstRefHolder {
            T const &value;

            EnumConstRefHolder(T const &value) : value(value) {}
        };

    } // oe::detail

    template<typename T>
    detail::EnumConstRefHolder<T> EnumToString(T const &e) { return detail::EnumConstRefHolder<T>(e); }

    template<typename T>
    detail::EnumRefHolder<T> EnumFromString(T &e) { return detail::EnumRefHolder<T>(e); }

    namespace detail {

        template<typename Enum>
        struct EnumSerializable {
            static const bool enable = false;
        };

        template<class Enum>
        inline constexpr bool EnumSerializableV = EnumSerializable<Enum>::enable;

        template<typename Enum>
        requires EnumSerializableV<Enum>
        std::ostream &operator<<(std::ostream &os, Enum e) {
            return os << EnumToString(e);
        }

        template<typename Enum>
        requires EnumSerializableV<Enum>
        std::istream &operator>>(std::istream &is, Enum &e) {
            return is >> EnumFromString(e);
        }

#define MAKE_SERIALIZABLE(x)            \
template <>                             \
struct EnumSerializable<x>              \
{                                       \
    static const bool enable = true;    \
};

    }

#define ENABLE_BITMASK_OPERATORS(x)  \
template<>                           \
struct EnableBitMaskOperators<x>     \
{                                    \
    static const bool enable = true; \
};

    template<typename Enum>
    struct EnableBitMaskOperators {
        static const bool enable = false;
    };

    template<class T>
    inline constexpr bool EnableBitMaskOperatorsV = EnableBitMaskOperators<T>::enable;

    template<typename Enum>
    requires EnableBitMaskOperatorsV<Enum>
    constexpr Enum operator~(Enum val) {
        using underlying = typename std::underlying_type<Enum>::type;

        return static_cast<Enum> (
                ~static_cast<underlying> ( val )
        );
    }

    template<typename Enum>
    requires EnableBitMaskOperatorsV<Enum>
    constexpr Enum operator&(Enum lhs, Enum rhs) {
        using underlying = typename std::underlying_type<Enum>::type;

        return static_cast<Enum> (
                static_cast<underlying> ( lhs ) &
                static_cast<underlying> ( rhs )
        );
    }

    template<typename Enum>
    requires EnableBitMaskOperatorsV<Enum>
    constexpr Enum operator|(Enum lhs, Enum rhs) {
        using underlying = typename std::underlying_type<Enum>::type;

        return static_cast<Enum> (
                static_cast<underlying> ( lhs ) |
                static_cast<underlying> ( rhs )
        );
    }

    template<typename Enum>
    requires EnableBitMaskOperatorsV<Enum>
    constexpr Enum operator^(Enum lhs, Enum rhs) {
        using underlying = typename std::underlying_type<Enum>::type;

        return static_cast<Enum> (
                static_cast<underlying> ( lhs ) ^
                static_cast<underlying> ( rhs )
        );
    }

    template<typename Enum>
    requires EnableBitMaskOperatorsV<Enum>
    constexpr Enum operator&=(Enum &lhs, Enum rhs) {
        using underlying = typename std::underlying_type<Enum>::type;

        lhs = static_cast<Enum> (
                static_cast<underlying> ( lhs ) &
                static_cast<underlying> ( rhs )
        );

        return lhs;
    }

    template<typename Enum>
    requires EnableBitMaskOperatorsV<Enum>
    constexpr Enum operator|=(Enum &lhs, Enum rhs) {
        using underlying = typename std::underlying_type<Enum>::type;

        lhs = static_cast<Enum> (
                static_cast<underlying> ( lhs ) |
                static_cast<underlying> ( rhs )
        );

        return lhs;
    }

    template<typename Enum>
    requires EnableBitMaskOperatorsV<Enum>
    constexpr Enum operator^=(Enum &lhs, Enum rhs) {
        using underlying = typename std::underlying_type<Enum>::type;

        lhs = static_cast<Enum> (
                static_cast<underlying> ( lhs ) ^
                static_cast<underlying> ( rhs )
        );

        return lhs;
    }

} // oe
