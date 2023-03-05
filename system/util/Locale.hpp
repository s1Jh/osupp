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

#include "Resource.hpp"

#include <filesystem>
#include <string>
#include <unordered_map>

namespace PROJECT_NAMESPACE {

namespace util
{

class Locale
{
    friend Resource<Locale> PROJECT_NAMESPACE::Load<Locale>(const std::filesystem::path &path);
public:
    [[nodiscard]] const std::string &getTranslation(const std::string &key) const;

    [[nodiscard]] const std::string &operator[](const std::string &key) const;

    [[nodiscard]] const std::string &getTimeLocale() const;

    [[nodiscard]] const std::string &getDateLocale() const;

    [[nodiscard]] const std::string &getDecimalSeparator() const;

    [[nodiscard]] const std::string &getLocName() const;

    [[nodiscard]] const std::string &getLocCredits() const;

    inline std::unordered_map<std::string, std::string>::iterator begin() { return translations.begin(); }
    inline std::unordered_map<std::string, std::string>::iterator end() { return translations.end(); }

private:
    std::string timeLocale{"HH:mm:ss"};
    std::string dateLocale{"YYYY/MM/dd"};
    std::string decimalSeparator{'.'};
    std::string locName{"Fallback"};
    std::string locCredits{TOSTRING(GAME_TITLE)};
    std::unordered_map<std::string, std::string> translations;
};

void SetDefaultLocale(const Resource<Locale> &newLocale);
const Resource<Locale> &DefaultLocale();

}

template<>
Resource<util::Locale> Load(const std::filesystem::path& path);

template<> const std::vector<std::string> Resource<util::Locale>::allowedExtensions;

#if defined(WINDOWS)
std::string operator "" _i18n(const char *key, unsigned long long);
#elif defined(LINUX)
std::string operator "" _i18n(const char *key, unsigned long);
#endif

}