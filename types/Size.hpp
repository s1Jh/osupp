#pragma once

#include <iostream>

#include "Vec2.hpp"
#include "define.hpp"

namespace GAME_TITLE {

    template<typename T> requires std::is_arithmetic_v<T>
    struct size {
        size() = default;

        size(T _w, T _h) : w(_w), h(_h) {}

        template<typename C>
        size(vec2d <C> v) : w((C) v.x), h((C) v.y) {}

        template<typename C>
        operator size<C>() const {
            return size<C>{(C) w, (C) h};
        }

        template<typename C>
        explicit operator vec2d<C>() const {
            return vec2d<C>{(C) w, (C) h};
        }

        T w, h;
    };

    template<typename T>
    std::ostream &operator<<(std::ostream &os, const size<T> &dt) {
        os << dt.w << "x" << dt.h;

        return os;
    }

    using isize = size<int>;
    using usize = size<unsigned int>;
    using fsize = size<float>;
    using dsize = size<double>;

    template<typename T1, typename T2>
    inline size<T1> operator*(const size<T1> &lhs, const size<T2> &rhs) {
        return size<T1>{lhs.w * rhs.w, lhs.h * rhs.h};
    }

    template<typename T1, typename T2>
    inline size<T1> operator/(const size<T1> &lhs, const size<T2> &rhs) {
        return size<T1>{lhs.w / rhs.w, lhs.h / rhs.h};
    }

    template<typename T1, typename T2>
    inline size<T1> operator*=(size<T1> &lhs, const size<T2> &rhs) {
        return size<T1>{lhs.w *= rhs.w, lhs.h *= rhs.h};
    }

    template<typename T1, typename T2>
    inline size<T1> operator/=(size<T1> &lhs, const size<T2> &rhs) {
        return size<T1>{lhs.w /= rhs.w, lhs.h /= rhs.h};
    }

    template<typename T1, typename T2>
    inline bool operator==(const size<T1> &lhs, const size<T2> &rhs) {
        return lhs.w == rhs.w && lhs.h == rhs.h;
    }

    template<typename T1, typename T2>
    inline bool operator!=(const size<T1> &lhs, const size<T2> &rhs) {
        return lhs.w != rhs.w || lhs.h != rhs.h;
    }

    template<typename T1, typename T2>
    requires std::is_arithmetic_v<T2>
    inline size<T1> operator*(const size<T1> &in, T2 scaler) {
        return size<T1>{in.w * (T1) scaler, in.h * (T1) scaler};
    }

    template<typename T1, typename T2>
    requires std::is_arithmetic_v<T2>
    inline size<T1> operator/(const size<T1> &in, T2 scaler) {
        return size<T1>{in.w / (T1) scaler, in.h / (T1) scaler};
    }
    template<typename T1, typename T2>
    requires std::is_arithmetic_v<T2>
    inline size<T1> operator/(T2 scaler, const size<T1> &in) {
        return size<T1>{ (T1) scaler / in.w, (T1) scaler / in.h };
    }

    template<typename T1, typename T2>
    requires std::is_arithmetic_v<T2>
    inline size<T1> operator*=(size<T1> &in, T2 scaler) {
        return size<T1>{in.w *= (T1) scaler, in.h *= (T1) scaler};
    }

    template<typename T1, typename T2>
    requires std::is_arithmetic_v<T2>
    inline size<T1> operator/=(size<T1> &in, T2 scaler) {
        return size<T1>{in.w /= (T1) scaler, in.h /= (T1) scaler};
    }

}
