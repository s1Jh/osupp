#pragma once

#include "define.hpp"

#include <concepts>
#include <memory>
#include <unordered_map>
#include <mutex>

NS_BEGIN

namespace detail
{
struct BaseSharedValue
{
};
}

template<typename T> requires std::is_default_constructible_v<T>
struct SharedValue: public detail::BaseSharedValue
{
    SharedValue();
    explicit SharedValue(T value);

    explicit operator bool() const
    {
        std::scoped_lock<std::mutex> lock(pointer->mutex);
        return (bool) pointer->value;
    }

    [[nodiscard]] T get();
    void set(T &&);

    SharedValue<T> &operator=(const T &right);
    SharedValue<T> &operator=(const SharedValue<T> &right);

private:
    template<typename U>
    struct Container
    {
        U value;
        std::mutex mutex;
    };
    std::shared_ptr<Container<T>> pointer;
};

template<typename T>
requires std::is_default_constructible_v<T> T SharedValue<T>::get()
{
    std::scoped_lock<std::mutex> lock(pointer->mutex);
    return pointer->value;
}

template<typename T>
requires std::is_default_constructible_v<T>
void SharedValue<T>::set(T &&valueIn)
{
    std::scoped_lock<std::mutex> lock(pointer->mutex);
    pointer->value = valueIn;
}

template<typename T>
requires std::is_default_constructible_v<T> SharedValue<T>::SharedValue()
{
    pointer = std::make_shared<Container<T>>();
}

template<typename T>
requires std::is_default_constructible_v<T>
SharedValue<T>::SharedValue(T valueIn)
{
    pointer = std::make_shared<Container<T>>(valueIn);
}

template<typename T>
requires std::is_default_constructible_v<T> SharedValue<T>
&SharedValue<T>::operator=(const T &right)
{
    std::scoped_lock<std::mutex> lock(right.pointer->mutex);
    *this = right;
    return *this;
}

template<typename T>
requires std::is_default_constructible_v<T> SharedValue<T>
&SharedValue<T>::operator=(const SharedValue<T> &right)
{
    std::scoped_lock<std::mutex> lock(pointer->mutex);
    if (this == &right)
        return *this;

    pointer = right.pointer;
    return *this;
}

NS_END