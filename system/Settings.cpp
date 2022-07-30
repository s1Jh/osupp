//
// Created by sijh on 27.07.22.
//

#include "Settings.hpp"
#include "Util.hpp"

NS_BEGIN

void Settings::iterateSettingsSearch(
    InactiveSettingStorageT &entries, const df2 &clump, const std::string &previousName)
{
    std::string thisName = previousName + clump.name();

    for (const auto &setting: clump) {
        const auto &value = setting.second;
        auto push = [&](const std::string &value)
        {
            entries[thisName + '.' + setting.first] = value;
        };

        std::string strRepr;
        switch (setting.second.getType()) {
            case df2::EntryType::Clump:
                iterateSettingsSearch(entries, setting.second, thisName);
                break;
            case df2::EntryType::Color:
                push(std::to_string(value.col().r) + ','
                         + std::to_string(value.col().g) + ','
                         + std::to_string(value.col().b) + ','
                         + std::to_string(value.col().a));
                break;
            case df2::EntryType::Vector:
                push(std::to_string(value.vec().x) + ';' + std::to_string(value.vec().y));
                break;

            case df2::EntryType::Integer:
                push(std::to_string(value.integer()));
                break;
            case df2::EntryType::Real:
                push(std::to_string(value.real()));
                break;
            case df2::EntryType::Boolean:
                push(value.boolean() ? "true" : "false");
                break;
            case df2::EntryType::String:
                push(value.str());
                break;
            default:
                break;
        }
    }
}

bool Settings::write(const std::filesystem::path &path) const
{
    auto df2 = dump();
    return df2::write(df2, path);
}

bool Settings::read(const std::filesystem::path &path)
{
    auto read = df2::read(path);
    iterateSettingsSearch(savedValues, read, "setting.");
    return !read.isEmpty();
}

detail::BaseSetting::BaseSetting(SettingType typeIn) noexcept
    : type(typeIn)
{}

void Settings::apply()
{
    log::debug("Invoking callbacks");
    invokeCallback<SettingCallbacks::SettingChanged>("");
}

SettingType detail::BaseSetting::getType() const
{
    return type;
}

Settings::ActiveSettingStorageT::iterator Settings::begin()
{
    return activeValues.begin();
}

Settings::ActiveSettingStorageT::iterator Settings::end()
{
    return activeValues.end();
}

Settings::ActiveSettingStorageT::const_iterator Settings::cbegin()
{
    return activeValues.cbegin();
}

Settings::ActiveSettingStorageT::const_iterator Settings::cend()
{
    return activeValues.cend();
}

df2 Settings::dump() const
{
    df2 ret;

    for (const auto &entry: activeValues) {
        const auto &key = entry.first;
        const auto &value = entry.second;

        std::string stringRepr;

        switch (value->getType()) {
            case SettingType::Boolean: {
                auto cast = std::static_pointer_cast<Setting<bool>>(value);
                if (cast->getMetadata().writeToConfig)
                    stringRepr = SettingMetadata<bool>::toString(cast->get());
                break;
            }
            case SettingType::Color: {
                auto cast = std::static_pointer_cast<Setting<color>>(value);
                if (cast->getMetadata().writeToConfig)
                    stringRepr = SettingMetadata<color>::toString(cast->get());
                break;
            }
            case SettingType::String: {
                auto cast = std::static_pointer_cast<Setting<std::string>>(value);
                if (cast->getMetadata().writeToConfig)
                    stringRepr = SettingMetadata<std::string>::toString(cast->get());
                break;
            }
            case SettingType::Integer: {
                auto cast = std::static_pointer_cast<Setting<int>>(value);
                if (cast->getMetadata().writeToConfig)
                    stringRepr = SettingMetadata<int>::toString(cast->get());
                break;

            }
            case SettingType::Float: {
                auto cast = std::static_pointer_cast<Setting<float>>(value);
                if (cast->getMetadata().writeToConfig)
                    stringRepr = SettingMetadata<float>::toString(cast->get());
                break;
            }
            case SettingType::None:
            default:
                break;
        }

        if (stringRepr.empty())
            continue;

        auto steps = GetCharacterSeparatedValues(key, '.');
        // Get rid of the "settings" bit we add during reading for localisation purposes
        steps.erase(steps.begin());

        auto &clump = GetClumpRecursive(steps, ret);
        clump.str() = stringRepr;
    }

    return ret;
}
df2 &Settings::GetClumpRecursive(std::vector<std::string> &steps, df2 &clump)
{
    if (steps.empty())
        return clump;

    auto next = steps.front();
    steps.erase(steps.begin());
    return GetClumpRecursive(steps, clump[next]);
}

NS_END