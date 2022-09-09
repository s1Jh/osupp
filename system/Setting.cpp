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

#include "Setting.hpp"
#include "Math.hpp"

NS_BEGIN

#define TO_STRING_FUNC(Type, SettingType) \
    template<> const detail::SettingMetadataFields<Type, SettingType>::ToStringFunction \
    detail::SettingMetadataFields<Type, SettingType>::toString = [](const Type& value) -> std::string

#define FROM_STRING_FUNC(Type, SettingType) \
    template<> const detail::SettingMetadataFields<Type, SettingType>::FromStringFunction \
    detail::SettingMetadataFields<Type, SettingType>::fromString = [](const std::string& string) -> Type

#define CONSTRAINT_FUNC(Type, SettingType) \
    template<> const detail::SettingMetadataFields<Type, SettingType>::ConstraintFunction \
    detail::SettingMetadataFields<Type, SettingType>::applyConstraints =                  \
    []([[maybe_unused]] const SettingMetadata<Type>& ctx, const Type& value) -> Type

TO_STRING_FUNC(float, SettingType::FLOAT)
{
    return std::to_string(value);
};

FROM_STRING_FUNC(float, SettingType::FLOAT)
{
    return std::strtof(string.c_str(), nullptr);
};

CONSTRAINT_FUNC(float, SettingType::FLOAT)
{
    return ctx.min != ctx.max ? Clamp(value, ctx.min, ctx.max) : value;
};

TO_STRING_FUNC(int, SettingType::INTEGER)
{
    return std::to_string(value);
};

FROM_STRING_FUNC(int, SettingType::INTEGER)
{
    return (int) std::strtol(string.c_str(), nullptr, 10);
};

CONSTRAINT_FUNC(int, SettingType::INTEGER)
{
    return ctx.min != ctx.max ? Clamp(value, ctx.min, ctx.max) : value;
};

TO_STRING_FUNC(color, SettingType::COLOR)
{
    return (std::string) value;
};

FROM_STRING_FUNC(color, SettingType::COLOR)
{
    color8 truncated;
    auto components = GetCharacterSeparatedValues(string, ',');
    truncated.r = Min(GetParam<int>(components, 0, 0), 255);
    truncated.g = Min(GetParam<int>(components, 1, 0), 255);
    truncated.b = Min(GetParam<int>(components, 2, 0), 255);
    truncated.a = Min(GetParam<int>(components, 3, 0), 255);
    return (color) truncated;
};

CONSTRAINT_FUNC(color, SettingType::COLOR)
{
    auto &palette = ctx.palette;
    // if there are no options to choose from, assume this setting to not
    // be restricted to a list of options and return whatever was passed in
    if (palette.empty())
        return value;

    // check if value is in allowed options, if it is, return it
    if (std::find(palette.begin(), palette.end(), value) != palette.end()) {
        return value;
    }

    // otherwise, return the first allowed value
    return palette[0];
};

TO_STRING_FUNC(bool, SettingType::BOOLEAN)
{
    return value ? "true" : "false";
};

FROM_STRING_FUNC(bool, SettingType::BOOLEAN)
{
    std::string conv = TrimCopy(string);
    std::transform(conv.begin(), conv.end(), conv.begin(), ::tolower);
    return conv == "true";
};

CONSTRAINT_FUNC(bool, SettingType::BOOLEAN)
{
    return value;
};

TO_STRING_FUNC(std::string, SettingType::STRING)
{
    return value;
};

FROM_STRING_FUNC(std::string, SettingType::STRING)
{
    return string;
};

CONSTRAINT_FUNC(std::string, SettingType::STRING)
{
    auto &options = ctx.options;
    // if there are no options to choose from, assume this setting to not
    // be restricted to a list of options and return whatever was passed in
    if (options.empty())
        return value;

    // check if value is in allowed options, if it is, return it
    if (std::find(options.begin(), options.end(), value) != options.end()) {
        return value;
    }

    // otherwise, return the first allowed value
    return options[0];
};

NS_END