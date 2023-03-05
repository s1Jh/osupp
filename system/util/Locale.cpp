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

#include "Locale.hpp"

#include "imgui.h"
#include "df2.hpp"

#include <mutex>

namespace PROJECT_NAMESPACE {

template<> const std::vector<std::string> Resource<util::Locale>::allowedExtensions = {".ldf"};

namespace util
{

static std::mutex LocaleMutex;

static Resource<Locale> CurrentDefaultLocale;

const std::string &Locale::getTimeLocale() const
{ return timeLocale; }

const std::string &Locale::getDateLocale() const
{ return dateLocale; }

const std::string &Locale::getDecimalSeparator() const
{
    return decimalSeparator;
}

const std::string &Locale::getLocName() const
{ return locName; }

const std::string &Locale::getLocCredits() const
{ return locCredits; }

const std::string &Locale::getTranslation(const std::string &key) const
{
    if (translations.find(key) != translations.end())
        return translations.at(key);
    else {
        return key;
    }
}

const std::string &Locale::operator[](const std::string &key) const
{
    return getTranslation(key);
}

void SetDefaultLocale(const Resource<Locale> &newLocale)
{
	std::lock_guard<std::mutex> lock(LocaleMutex);
    CurrentDefaultLocale = newLocale;
}

const Resource<Locale> &DefaultLocale()
{
	return CurrentDefaultLocale;
}

}

#if defined(WINDOWS)
std::string operator "" _i18n(const char *key, unsigned long long)
#elif defined(LINUX)
std::string operator "" _i18n(const char *key, unsigned long)
#endif
{
	return util::DefaultLocale()->getTranslation(key);
}

template<>
Resource<util::Locale> Load(const std::filesystem::path& path)
{
    Resource<util::Locale> r;

    df2 parser = df2::read(path);

    if (parser.find("locale") == parser.end()) {
        return {nullptr};
    }

    auto &clump = parser["locale"];
    r->dateLocale = clump["date"].str("dd/MM/yyyy");
    r->timeLocale = clump["time"].str("HH:mm");
    r->decimalSeparator = clump["decimal_sep"].str(".");
    r->locName = clump["name"].str("unknown");
    r->locCredits = clump["credits"].str("unknown");

    auto &translationClumps = clump["translations"];
    for (auto &translationClump: translationClumps) {
        if (translationClump.second.getType() == df2::EntryType::STRING) {
            r->translations[translationClump.first] = std::move(translationClump.second.str());
        }
    }

	return r;
}

}