/*
 Copyright (c) 2023 sijh

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "define.hpp"

#include <string>

#include "ToFromString.hpp"

namespace PROJECT_NAMESPACE
{

template <typename Arg>
std::string Concatenate(Arg arg)
{
    return ToString<Arg>(arg);
}
template <typename Arg1, typename ... Args>
std::string Concatenate(Arg1 arg1, Args... args)
{
    auto str = ToString<Arg1>(arg1) + Concatenate(args...);
    return str;
}

std::size_t ReplaceAll(std::string& inout, std::string_view what, std::string_view with);
std::size_t RemoveAll(std::string& inout, std::string_view what);

void LowerCase(std::string &s);
void UpperCase(std::string &s);
void SwitchCase(std::string &s);

std::string LowerCaseCopy(std::string s);
std::string UpperCaseCopy(std::string s);
std::string SwitchCaseCopy(std::string s);

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

}
