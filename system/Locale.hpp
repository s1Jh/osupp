#pragma once

#include "define.hpp"

#include <filesystem>
#include <string>
#include <unordered_map>

NS_BEGIN

class Locale
{
public:
    int loadFromFile(const std::filesystem::path &path);

    const std::string &getTranslation(const std::string &key) const;

    const std::string &operator[](const std::string &key) const;

    const std::string &getTimeLocale() const;

    const std::string &getDateLocale() const;

    const std::string &getDecimalSeparator() const;

    const std::string &getLocName() const;

    const std::string &getLocCredits() const;

    void showDebugListings(bool *open = nullptr) const;

private:
    std::string timeLocale;
    std::string dateLocale;
    std::string decimalSeparator;
    std::string locName;
    std::string locCredits;
    std::unordered_map<std::string, std::string> translations;
};

NS_END