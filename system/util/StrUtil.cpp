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

#include "StrUtil.hpp"

#include <algorithm> 
#include <locale>
#include <codecvt>

namespace PROJECT_NAMESPACE
{

    std::size_t ReplaceAll(std::string &inout, std::string_view what, std::string_view with)
    {
        std::size_t count{};
        for (std::string::size_type pos{};
             std::string::npos != (pos = inout.find(what.data(), pos, what.length()));
             pos += with.length(), ++count)
        {
            inout.replace(pos, what.length(), with.data(), with.length());
        }
        return count;
    }

    std::size_t RemoveAll(std::string &inout, std::string_view what)
    {
        return ReplaceAll(inout, what, "");
    }

    /**
     * String trimming functions from https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring/217605#217605
     */

    void LTrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                        { return !std::isspace(ch); }));
    }

    void RTrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                             { return !std::isspace(ch); })
                    .base(),
                s.end());
    }

    void Trim(std::string &s)
    {
        LTrim(s);
        RTrim(s);
    }

    std::string LTrimCopy(std::string s)
    {
        LTrim(s);
        return s;
    }

    std::string RTrimCopy(std::string s)
    {
        RTrim(s);
        return s;
    }

    std::string TrimCopy(std::string s)
    {
        Trim(s);
        return s;
    }

    std::string ToUTF8(const std::u16string &s)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
        return conv.to_bytes(s);
    }

    std::string ToUTF8(const std::u32string &s)
    {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
        return conv.to_bytes(s);
    }

    std::u16string ToUTF16(const std::string &s)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
        return conv.from_bytes(s);
    }

    std::u16string ToUTF16(const std::u32string &s)
    {
        std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> conv;
        std::string bytes = conv.to_bytes(s);
        return {reinterpret_cast<const char16_t *>(bytes.c_str()), bytes.length() / sizeof(char16_t)};
    }

    std::u32string ToUTF32(const std::string &s)
    {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
        return conv.from_bytes(s);
    }

    std::u32string ToUTF32(const std::u16string &s)
    {
        const char16_t *pData = s.c_str();
        std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> conv;
        return conv.from_bytes(reinterpret_cast<const char *>(pData), reinterpret_cast<const char *>(pData + s.length()));
    }

    void LowerCase(std::string &s)
    {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    }

    void UpperCase(std::string &s)
    {
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    }

    void SwitchCase(std::string &s)
    {
        auto Switch = [](int c) -> int
        {
            if (isupper(c))
            {
                return tolower(c);
            }
            else if (islower(c))
            {
                return toupper(c);
            }
            return c;
        };
        std::transform(s.begin(), s.end(), s.begin(), Switch);
    }

    std::string LowerCaseCopy(std::string s)
    {
        LowerCase(s);
        return s;
    }

    std::string UpperCaseCopy(std::string s)
    {
        UpperCase(s);
        return s;
    }

    std::string SwitchCaseCopy(std::string s)
    {
        SwitchCase(s);
        return s;
    }

}