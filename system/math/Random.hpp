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

#include "define.hpp"

#include "Vec2.hpp"
#include "Vec3.hpp"
#include "Vec4.hpp"

#include <limits>
#include <random>
#include <type_traits>

NS_BEGIN

namespace math
{

namespace Random
{

template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
Scalar(T min = std::numeric_limits<T>::min(),
	   T max = std::numeric_limits<T>::max());

template<typename T>
vec2d<T> Vec2(vec2d<T> min = {std::numeric_limits<T>::min(),
							  std::numeric_limits<T>::min()},
			  vec2d<T> max = {std::numeric_limits<T>::max(),
							  std::numeric_limits<T>::max()});

template<typename T>
vec3d<T> Vec3(vec3d<T> min = {std::numeric_limits<T>::min(),
							  std::numeric_limits<T>::min()},
			  vec3d<T> max = {std::numeric_limits<T>::max(),
							  std::numeric_limits<T>::max()});

template<typename T>
vec4d<T> Vec4(vec4d<T> min = {std::numeric_limits<T>::min(),
							  std::numeric_limits<T>::min()},
			  vec4d<T> max = {std::numeric_limits<T>::max(),
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
	} else {
		std::uniform_int_distribution<T> dis(min, max);
		return static_cast<T>(dis(generator));
	}
}

template<typename T>
vec2d<T> Random::Vec2(vec2d<T> min, vec2d<T> max)
{
	return vec2d<T>{Random::Scalar<T>(min.x, max.x),
					Random::Scalar<T>(min.y, max.y)};
}

template<typename T>
vec3d<T> Random::Vec3(vec3d<T> min, vec3d<T> max)
{
	return vec3d<T>{Random::Scalar<T>(min.x, max.x),
					Random::Scalar<T>(min.y, max.y),
					Random::Scalar<T>(min.z, max.z)};
}

template<typename T>
vec4d<T> Random::Vec4(vec4d<T> min, vec4d<T> max)
{
	return vec4d<T>{
		Random::Scalar<T>(min.x, max.x), Random::Scalar<T>(min.y, max.y),
		Random::Scalar<T>(min.z, max.z), Random::Scalar<T>(min.w, max.w)};
}

template<typename T>
requires std::is_floating_point_v<T>
T Perlin2D(T x, T y, unsigned int octaves, double persistence,
		   int seed, double amplitude)
{
	T ret = 0;

	int ix = (int)x;
	int iy = (int)y;

	int s = (ix + iy) * (ix + iy + 1) / 2 + iy + seed;

	for (unsigned int i = 0; i < octaves; i++) {
		std::srand(s);
		ret += amplitude * Random::Scalar<T>(0.0, 1.0);
		amplitude *= persistence;
	}
	return ret;
}

}

NS_END
