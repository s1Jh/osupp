#pragma once

#include <limits>
#include <random>
#include <type_traits>

#include "Vec2.hpp"
#include "Vec3.hpp"
#include "Vec4.hpp"
#include "define.hpp"

NS_BEGIN

namespace Random
{

template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
Scalar(T min = std::numeric_limits<T>::min(),
       T max = std::numeric_limits<T>::max());

template<typename T>
vec2d <T> Vec2(vec2d <T> min = {std::numeric_limits<T>::min(),
                                std::numeric_limits<T>::min()},
               vec2d <T> max = {std::numeric_limits<T>::max(),
                                std::numeric_limits<T>::max()});

template<typename T>
vec3d <T> Vec3(vec3d <T> min = {std::numeric_limits<T>::min(),
                                std::numeric_limits<T>::min()},
               vec3d <T> max = {std::numeric_limits<T>::max(),
                                std::numeric_limits<T>::max()});

template<typename T>
vec4d <T> Vec4(vec4d <T> min = {std::numeric_limits<T>::min(),
                                std::numeric_limits<T>::min()},
               vec4d <T> max = {std::numeric_limits<T>::max(),
                                std::numeric_limits<T>::max()});

} // namespace Random

template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
Random::Scalar(T min, T max)
{
    std::random_device seeder;
    std::mt19937_64 generator(seeder());

    if constexpr (std::is_floating_point<T>::value) {
        std::uniform_real_distribution<T> dis(min, max);
        return static_cast<T>(dis(generator));
    }
    else {
        std::uniform_int_distribution<T> dis(min, max);
        return static_cast<T>(dis(generator));
    }
}

template<typename T>
vec2d <T> Random::Vec2(vec2d <T> min, vec2d <T> max)
{
    return vec2d<T>{Random::Scalar<T>(min.x, max.x),
                    Random::Scalar<T>(min.y, max.y)};
}

template<typename T>
vec3d <T> Random::Vec3(vec3d <T> min, vec3d <T> max)
{
    return vec3d<T>{Random::Scalar<T>(min.x, max.x),
                    Random::Scalar<T>(min.y, max.y),
                    Random::Scalar<T>(min.z, max.z)};
}

template<typename T>
vec4d <T> Random::Vec4(vec4d <T> min, vec4d <T> max)
{
    return vec4d<T>{
        Random::Scalar<T>(min.x, max.x), Random::Scalar<T>(min.y, max.y),
        Random::Scalar<T>(min.z, max.z), Random::Scalar<T>(min.w, max.w)};
}

NS_END
