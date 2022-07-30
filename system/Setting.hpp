#pragma once

#include "define.hpp"

#include "Color.hpp"

#include <vector>
#include <string>
#include <functional>
#include <mutex>
#include <memory>

NS_BEGIN

enum class SettingType
{
    None,
    Boolean,      // will be shown as a checkbox
    Integer,
    Float,        // will be shown as a slider
    String,       // will be shown as a tex box
    Color,        // will be shown with a color picker
};

namespace detail
{
class BaseSetting
{
public:
    explicit BaseSetting(SettingType typeIn) noexcept;

    [[nodiscard]] SettingType getType() const;

private:
    const SettingType type;
};
}

template<typename U>
struct SettingMetadata
{
    const static bool enable = false;
};

namespace detail
{

template<typename U, SettingType Type>
struct SettingMetadataFields
{
    const static SettingType type = Type;
    const static bool enable = true;

    using ToStringFunction = std::function<std::string(const U &)>;
    using FromStringFunction = std::function<U(const std::string &)>;
    using ConstraintFunction = std::function<U(const SettingMetadata<U> &, const U &)>;

    const static ToStringFunction toString;
    const static FromStringFunction fromString;
    const static ConstraintFunction applyConstraints;

    U initial{};
    bool writeToConfig{true};
};

}

template<>
struct SettingMetadata<float>: public detail::SettingMetadataFields<float, SettingType::Float>
{
    float min{0};
    float max{1};
    bool slider{true};
};

template<>
struct SettingMetadata<int>: public detail::SettingMetadataFields<int, SettingType::Integer>
{
    int min{0};
    int max{1};
};

template<>
struct SettingMetadata<color>: public detail::SettingMetadataFields<color, SettingType::Color>
{
    std::vector<color> palette = {};
};

template<>
struct SettingMetadata<bool>: public detail::SettingMetadataFields<bool, SettingType::Boolean>
{
};

template<>
struct SettingMetadata<std::string>: public detail::SettingMetadataFields<std::string, SettingType::String>
{
    std::vector<std::string> options = {};
};

template<typename T> requires std::is_default_constructible_v<T>
class Setting: public detail::BaseSetting
{
public:
    explicit Setting();
    explicit Setting(SettingMetadata<T> meta);
    explicit Setting(T value, SettingMetadata<T> meta);

    explicit operator bool() const
    {
        return (bool) held;
    }

    T get() const;
    T *getPtr() const;
    void set(const T &);
    void set(T &&);
    void reset();

    SettingMetadata<T> getMetadata();

    Setting<T> &operator=(const T &right);
    Setting<T> &operator=(const Setting<T> &right);

private:
    template<typename U>
    struct SettingContainer
    {
        U value;
        SettingMetadata<U> meta;
        std::mutex mutex{};
    };

    std::shared_ptr<SettingContainer<T>> held;
};

template<typename T>
requires std::is_default_constructible_v<T>
Setting<T>::Setting()
    : BaseSetting(SettingType::None), held(nullptr)
{}

template<typename T>
requires std::is_default_constructible_v<T>
Setting<T>::Setting(T value, SettingMetadata<T> meta)
    :
    BaseSetting(meta.type)
{
    held = std::make_shared<SettingContainer<T>>(T{}, std::move(meta));
    // FIXME: We're setting the value twice, once during construction then again through set()
    //        to properly apply value constraints.
    set(value);
}

template<typename T>
requires std::is_default_constructible_v<T>
Setting<T>::Setting(SettingMetadata<T> meta)
    :
    BaseSetting(meta.type)
{
    held = std::make_shared<SettingContainer<T>>(std::move(T{}), std::move(meta));
    set(held->meta.initial);
}

template<typename T>
requires std::is_default_constructible_v<T>
T Setting<T>::get() const
{
    std::scoped_lock<std::mutex> lock(held->mutex);
    return held->value;
}

template<typename T>
requires std::is_default_constructible_v<T>
void Setting<T>::set(const T &newValue)
{
    std::scoped_lock<std::mutex> lock(held->mutex);
    held->value = held->meta.applyConstraints(held->meta, newValue);
}

template<typename T>
requires std::is_default_constructible_v<T>
void Setting<T>::set(T &&newValue)
{
    std::scoped_lock<std::mutex> lock(held->mutex);
    held->value = held->meta.applyConstraints(held->meta, newValue);
}

template<typename T>
requires std::is_default_constructible_v<T>
void Setting<T>::reset()
{
    std::scoped_lock<std::mutex> lock(held->mutex);
    set(held->meta.initial);
}

template<typename T>
requires std::is_default_constructible_v<T>
SettingMetadata<T> Setting<T>::getMetadata()
{
    std::scoped_lock<std::mutex> lock(held->mutex);
    return held->meta;
}

template<typename T>
requires std::is_default_constructible_v<T>
Setting<T> &Setting<T>::operator=(const T &right)
{
    std::scoped_lock<std::mutex> lock(held->mutex);
    set(right);
    return *this;
}

template<typename T>
requires std::is_default_constructible_v<T>
Setting<T> &Setting<T>::operator=(const Setting<T> &right)
{
    std::scoped_lock<std::mutex> lock(right.held->mutex);
    if (this == &right)
        return *this;

    held = right.held;
    return *this;
}

template<typename T>
requires std::is_default_constructible_v<T>
T *Setting<T>::getPtr() const
{
    return &held->value;
}

NS_END