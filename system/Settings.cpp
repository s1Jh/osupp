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

//
// Created by sijh on 27.07.22.
//

#include "Settings.hpp"
#include "Util.hpp"
#include "Setting.hpp"


NS_BEGIN

void Settings::iterateSettingsSearch(
    InactiveSettingStorageT &entries, const df2 &clump, const std::string &previousName)
{
    std::string thisName = previousName + clump.name();

    for (const auto &setting: clump) {
        const auto &value = setting.second;
        auto push = [&](const std::string &value)
        {
			auto name = thisName + '.' + setting.first;
            entries[name] = value;
			if (activeValues.contains(name)) {
				auto& existing = activeValues[name];
				SetSettingFromString(existing, value);
			}
        };

        std::string strRepr;
        switch (setting.second.getType()) {
            case df2::EntryType::CLUMP:
                iterateSettingsSearch(entries, setting.second, thisName);
                break;
            case df2::EntryType::COLOR:
                push(std::to_string(value.col().r) + ','
                         + std::to_string(value.col().g) + ','
                         + std::to_string(value.col().b) + ','
                         + std::to_string(value.col().a));
                break;
            case df2::EntryType::VECTOR:
                push(std::to_string(value.vec().x) + ';' + std::to_string(value.vec().y));
                break;

            case df2::EntryType::INTEGER:
                push(std::to_string(value.integer()));
                break;
            case df2::EntryType::REAL:
                push(std::to_string(value.real()));
                break;
            case df2::EntryType::BOOLEAN:
                push(value.boolean() ? "true" : "false");
                break;
            case df2::EntryType::STRING:
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

	for (const auto& setting : activeValues) {
		if (setting.second->wasChanged()) {
			log::debug("Invoking ", setting.first);
			invokeCallback<SettingCallbacks::SETTING_CHANGED>(setting.first);
		}
	}

}

SettingType detail::BaseSetting::getType() const
{
    return type;
}

bool detail::BaseSetting::wasChanged()
{
	return false;
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

        std::string stringRepr = GetStringFromSetting(value);

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

void SetSettingFromString(std::shared_ptr<detail::BaseSetting> &setting, const std::string &value)
{
	switch (setting->getType()) {

#define SETTING_TYPE(Name, Storage ) \
	case SettingType::Name: { \
		auto cast = std::static_pointer_cast<Setting<Storage>>(setting); \
		if (!bool(cast->getMetadata().flags & SettingFlags::READONLY)) \
			cast->set(SettingMetadata<Storage>::fromString(value)); \
		break; \
	}

	SETTING_TYPES
#undef SETTING_TYPE
	case SettingType::NONE:
	default:
		break;
	}
}

std::string GetStringFromSetting(const std::shared_ptr<detail::BaseSetting> &value)
{
	switch (value->getType()) {

#define SETTING_TYPE(Name, Storage ) \
	case SettingType::Name: { \
		auto cast = std::static_pointer_cast<Setting<Storage>>(value); \
		if (bool(cast->getMetadata().flags & SettingFlags::WRITE_TO_FILE)) \
			return SettingMetadata<Storage>::toString(cast->get()); \
		return ""; \
	}

	SETTING_TYPES
#undef SETTING_TYPE
	case SettingType::NONE:
	default:
		return "";
	}
}

NS_END
