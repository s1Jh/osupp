#pragma once

#include "define.hpp"

#include <filesystem>
#include <map>
#include <unordered_map>

#include "CallbackManager.hpp"
#include "Math.hpp"
#include "Setting.hpp"
#include "df2.hpp"

NS_BEGIN

enum class SettingCallbacks
{
    SettingChanged
};

MAKE_CALLBACK(SettingCallbacks, SettingCallbacks::SettingChanged, const std::string&)

class Settings: public detail::Callbacks<SettingCallbacks>
{
public:
    using ActiveSettingStorageT = std::map<std::string, std::shared_ptr<detail::BaseSetting>>;
    using InactiveSettingStorageT = std::unordered_map<std::string, std::string>;
    // Settings access functions. When called, they will first check if a valid Setting object has already been
    // created, if not they will create a new one and format it as necessary. Once it is created or if it already
    // exists, the corresponding SharedValue object will be returned which the caller may then use to access
    // the value without having to re-do the lookup each time.
    template<typename T>
    Setting<T> getSetting(const std::string &key);
    template<typename T, typename ... InitArgs>
    Setting<T> addSetting(const std::string &key, InitArgs... args);

    // read or write the settings to disk
    bool write(const std::filesystem::path &path = CONFIG_PATH) const;
    bool read(const std::filesystem::path &path = CONFIG_PATH);

    ActiveSettingStorageT::iterator begin();
    ActiveSettingStorageT::iterator end();
    ActiveSettingStorageT::const_iterator cbegin();
    ActiveSettingStorageT::const_iterator cend();

    void apply();

    df2 dump() const;

private:
    static df2 &GetClumpRecursive(std::vector<std::string> &steps, df2 &clump);

    void iterateSettingsSearch(
        InactiveSettingStorageT &entries, const df2 &clump, const std::string &previousName);

    // All variables read from and then back to the game configuration file will be kept here
    InactiveSettingStorageT savedValues;
    // Converted values
    ActiveSettingStorageT activeValues;
};

template<typename T>
Setting<T> Settings::getSetting(const std::string &key)
{
    if (activeValues.contains(key)) {
        auto &value = activeValues.at(key);
        if (value->getType() == SettingMetadata<T>::type) {
            auto newPtr = std::static_pointer_cast<Setting<T>>(value);
            return *newPtr;
        }
    }
    return Setting<T>{};
}

template<typename T, typename ... InitArgs>
Setting<T> Settings::addSetting(const std::string &key, InitArgs... args)
{
    SettingMetadata<T>::type;
    if (!activeValues.contains(key)) {
        SettingMetadata<T> meta = {args...};
        T value = meta.initial;

        if (savedValues.contains(key)) {
            value = meta.fromString(savedValues[key]);
        }

        auto ptr = std::make_shared<Setting<T>>(value, meta);
        auto basePtr = std::static_pointer_cast<detail::BaseSetting>(ptr);
        activeValues[key] = basePtr;
    }

    return getSetting<T>(key);
}

NS_END