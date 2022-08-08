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