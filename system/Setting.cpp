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
    [](const SettingMetadata<Type>& ctx,const Type& value) -> Type

TO_STRING_FUNC(float, SettingType::Float)
{
    return std::to_string(value);
};

FROM_STRING_FUNC(float, SettingType::Float)
{
    return std::strtof(string.c_str(), nullptr);
};

CONSTRAINT_FUNC(float, SettingType::Float)
{
    return ctx.min != ctx.max ? Clamp(value, ctx.min, ctx.max) : value;
};

TO_STRING_FUNC(int, SettingType::Integer)
{
    return std::to_string(value);
};

FROM_STRING_FUNC(int, SettingType::Integer)
{
    return (int) std::strtol(string.c_str(), nullptr, 10);
};

CONSTRAINT_FUNC(int, SettingType::Integer)
{
    return ctx.min != ctx.max ? Clamp(value, ctx.min, ctx.max) : value;
};

TO_STRING_FUNC(color, SettingType::Color)
{
    return (std::string) value;
};

FROM_STRING_FUNC(color, SettingType::Color)
{
    color8 truncated;
    auto components = GetCharacterSeparatedValues(string, ',');
    truncated.r = Min(GetParam<int>(components, 0, 0), 255);
    truncated.g = Min(GetParam<int>(components, 1, 0), 255);
    truncated.b = Min(GetParam<int>(components, 2, 0), 255);
    truncated.a = Min(GetParam<int>(components, 3, 0), 255);
    return (color) truncated;
};

CONSTRAINT_FUNC(color, SettingType::Color)
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

TO_STRING_FUNC(bool, SettingType::Boolean)
{
    return value ? "true" : "false";
};

FROM_STRING_FUNC(bool, SettingType::Boolean)
{
    std::string conv = TrimCopy(string);
    std::transform(conv.begin(), conv.end(), conv.begin(), ::tolower);
    return conv == "true";
};

CONSTRAINT_FUNC(bool, SettingType::Boolean)
{
    return value;
};

TO_STRING_FUNC(std::string, SettingType::String)
{
    return value;
};

FROM_STRING_FUNC(std::string, SettingType::String)
{
    return string;
};

CONSTRAINT_FUNC(std::string, SettingType::String)
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