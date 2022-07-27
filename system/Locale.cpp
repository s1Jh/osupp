#include "Locale.hpp"

#include "Util.hpp"
#include "df2.hpp"

NS_BEGIN

int Locale::loadFromFile(const std::filesystem::path &path)
{
    df2 parser = df2::read(path);

    auto &clump = parser["locale"];

    dateLocale = clump["date"].str("dd/MM/yyyy");
    timeLocale = clump["time"].str("HH:mm");
    decimalSeparator = clump["decimal_sep"].str(".");
    locName = clump["name"].str("unknown");
    locCredits = clump["credits"].str("unknown");

    auto &translationClumps = clump["translations"];
    for (auto &translationClump: translationClumps) {
        if (translationClump.second.getType() == df2::EntryType::String) {
            translations[translationClump.first] =
                std::move(translationClump.second.str());
            log::debug("[LOCALE] Added locale entry ", translationClump.first);
        }
    }
    return 0;
}

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

NS_END