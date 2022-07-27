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

unsigned int CheckErrors(const std::string &file, int line,
                         const std::string &helper = "");
}

#define CheckGLFW detail::CheckGLFWErrors(__FILE__, __LINE__)
#define CheckGL detail::CheckErrors(__FILE__, __LINE__)

#define CheckGLFWh(_helper) detail::CheckGLFWErrors(__FILE__, __LINE__, _helper)
#define CheckGLh(_helper) detail::CheckErrors(__FILE__, __LINE__, _helper)

unsigned int DumpGlErrors();

std::vector<std::string> GetCharacterSeparatedValues(const std::string &in, char sep);

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
