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

#include "Color.hpp"
#include "EnumOperators.hpp"

#include <vector>
#include <string>
#include <functional>
#include <mutex>
#include <memory>

NS_BEGIN

#define SETTING_TYPES \
	SETTING_TYPE(BOOLEAN, bool) \
	SETTING_TYPE(INTEGER, int) \
	SETTING_TYPE(FLOAT, float) \
	SETTING_TYPE(STRING, std::string) \
	SETTING_TYPE(COLOR, color)

enum class SettingType
{
    NONE,
#define SETTING_TYPE(Name, Storage) Name,
	SETTING_TYPES
#undef SETTING_TYPE
};

enum class SettingFlags {
	NONE = 0,
	WRITE_TO_FILE = 1 << 0,
	READONLY = 1 << 1,
	HIDDEN = 1 << 2,
};

ENABLE_BITMASK_OPERATORS(SettingFlags);

constexpr SettingFlags DEFAULT_SETTING_FLAGS = SettingFlags::WRITE_TO_FILE | SettingFlags::READONLY;

namespace detail
{
class BaseSetting
{
public:
    explicit BaseSetting(SettingType typeIn) noexcept;

    [[nodiscard]] SettingType getType() const;

	virtual bool wasChanged();
	[[nodiscard]] virtual bool changed() const;
	[[nodiscard]] virtual SettingFlags flags() const;

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
	SettingFlags flags{DEFAULT_SETTING_FLAGS};
};

}

template<>
struct SettingMetadata<float>: public detail::SettingMetadataFields<float, SettingType::FLOAT>
{
    float min{0.0f};
    float max{1.0f};
    bool slider{true};
};

template<>
struct SettingMetadata<int>: public detail::SettingMetadataFields<int, SettingType::INTEGER>
{
    int min{0};
    int max{1};
};

template<>
struct SettingMetadata<color>: public detail::SettingMetadataFields<color, SettingType::COLOR>
{
    std::vector<color> palette = {};
};

template<>
struct SettingMetadata<bool>: public detail::SettingMetadataFields<bool, SettingType::BOOLEAN>
{
};

template<>
struct SettingMetadata<std::string>: public detail::SettingMetadataFields<std::string, SettingType::STRING>
{
    std::vector<std::string> options = {};
};

template<typename T> requires std::is_default_constructible_v<T>
class Setting: public detail::BaseSetting
{
	friend class Settings;
public:
    explicit Setting();
    explicit Setting(SettingMetadata<T> meta);
    explicit Setting(T value, SettingMetadata<T> meta);
	Setting(const Setting& right);

    explicit operator bool() const
    {
        return (bool) held;
    }

    T get() const;
    T *getPtr() const;
    void set(const T &);
    void set(T &&);
    void reset();

    SettingMetadata<T> getMetadata() const;

	[[nodiscard]] SettingFlags flags() const override;

	[[nodiscard]] bool changed() const override;

    Setting<T> &operator=(const T &right);
    Setting<T> &operator=(Setting<T> &&right) noexcept;

private:
	bool wasChanged() override;
    template<typename U>
    struct SettingContainer
    {
        U value;
        SettingMetadata<U> meta;
        std::mutex mutex{};
		bool changed{false};
    };

    std::shared_ptr<SettingContainer<T>> held;
};

template<typename T>
requires std::is_default_constructible_v<T>
Setting<T>::Setting()
    : BaseSetting(SettingType::NONE), held(nullptr)
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
    held->value = held->meta.applyConstraints(held->meta, held->meta.initial);
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
	if (held->value != newValue) {
		held->value = held->meta.applyConstraints(held->meta, newValue);
		held->changed = true;
	}
}

template<typename T>
requires std::is_default_constructible_v<T>
void Setting<T>::set(T &&newValue)
{
    std::scoped_lock<std::mutex> lock(held->mutex);
    auto newVal = held->meta.applyConstraints(held->meta, newValue);
    if (held->value != newVal) {
        held->value = newVal;
        held->changed = true;
    }
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
SettingMetadata<T> Setting<T>::getMetadata() const
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
Setting<T>::Setting(const Setting &right) : BaseSetting(right.getType())
{
	if (right.held)
		std::scoped_lock<std::mutex> lock(right.held->mutex);

	held = right.held;
}

template<typename T>
requires std::is_default_constructible_v<T>
Setting<T> &Setting<T>::operator=(Setting<T> &&right) noexcept
{
	if (right.held)
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

template<typename T>
requires std::is_default_constructible_v<T>
bool Setting<T>::wasChanged()
{
	auto oldValue = held->changed;
	held->changed = false;
	return oldValue;
}

template<typename T>
requires std::is_default_constructible_v<T>
SettingFlags Setting<T>::flags() const
{
	return held->meta.flags;
}

template<typename T>
requires std::is_default_constructible_v<T>
bool Setting<T>::changed() const
{
	return held->changed;
}

NS_END