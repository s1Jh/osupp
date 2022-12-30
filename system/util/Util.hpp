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

#include "Log.hpp"

#include <vector>
#include <locale>
#include <codecvt>

NS_BEGIN

namespace detail
{
int CheckGLFWErrors(const std::string &file, int line,
                    const std::string &helper = "");

unsigned int CheckGLErrors(const std::string &file, int line,
                         const std::string &helper = "");

unsigned int CheckALErrors(const std::string &file, int line,
						   const std::string &helper = "");
}

#define CheckGLFW PROJECT_NAMESPACE::detail::CheckGLFWErrors(__FILE__, __LINE__)
#define CheckGL PROJECT_NAMESPACE::detail::CheckGLErrors(__FILE__, __LINE__)
#define CheckAL PROJECT_NAMESPACE::detail::CheckALErrors(__FILE__, __LINE__)

#define CheckGLFWh(_helper) PROJECT_NAMESPACE::detail::CheckGLFWErrors(__FILE__, __LINE__, _helper)
#define CheckGLh(_helper) PROJECT_NAMESPACE::detail::CheckGLErrors(__FILE__, __LINE__, _helper)
#define CheckALh(_helper) PROJECT_NAMESPACE::detail::CheckALErrors(__FILE__, __LINE__, _helper)

unsigned int DumpGlErrors();

std::vector<std::string> GetCharacterSeparatedValues(const std::string &in, char sep);

template<typename T>
static T GetParam(const std::vector<std::string> &params, unsigned int param, T backup) noexcept
{
    if (param >= params.size())
        return backup;

    if constexpr (std::is_same_v<T, std::string>)
        return params[param];
    else if constexpr (std::is_arithmetic_v<T>)
        return static_cast<T>(std::strtod(params[param].c_str(), nullptr));

    return backup;
}

/**
 * String trimming functions from https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring/217605#217605
 */

void LTrim(std::string &s);
void RTrim(std::string &s);
void Trim(std::string &s);

std::string LTrimCopy(std::string s);
std::string RTrimCopy(std::string s);
std::string TrimCopy(std::string s);

std::string ToUTF8(const std::u16string &s);
std::string ToUTF8(const std::u32string &s);

std::u16string ToUTF16(const std::string &s);
std::u16string ToUTF16(const std::u32string &s);

std::u32string ToUTF32(const std::string &s);
std::u32string ToUTF32(const std::u16string &s);

NS_END
