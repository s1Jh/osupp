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

void SetSettingFromString(std::shared_ptr<detail::BaseSetting>& setting, const std::string& value);
std::string GetStringFromSetting(const std::shared_ptr<detail::BaseSetting>& setting);

template <typename T, typename VisitorReturnT = void, typename ... Arguments>
typename std::conditional<std::is_same_v<VisitorReturnT, void>, bool, std::optional<VisitorReturnT>>::type
VisitSetting(
	std::shared_ptr<detail::BaseSetting>& setting,
	const std::function<VisitorReturnT(Setting<T>&, Arguments...)>& visitor,
	Arguments... arguments)
{
	if (setting->getType() == SettingMetadata<T>::type) {
		auto cast = std::static_pointer_cast<Setting<T>>(setting);
		if constexpr(std::is_same_v<VisitorReturnT, void>) {
			visitor(*cast, arguments...);
			return true;
		}
		else
			return visitor(*cast, arguments...);
	}
	if constexpr(std::is_same_v<VisitorReturnT, void>)
		return false;
	else
		return {false};
}

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