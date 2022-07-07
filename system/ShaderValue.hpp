#pragma once

#include <concepts>
#include <memory>
#include <unordered_map>

template<typename T> requires std::is_default_constructible_v<T>
struct SharedValue
{
    SharedValue();
    SharedValue(T value);

    [[nodiscard]] T get() const;
    void set(T &&);

    SharedValue<T> &operator=(const T &right);
    SharedValue<T> &operator=(const SharedValue<T> &right);

private:
    std::shared_ptr<T> value;
};

template<typename T>
requires std::is_default_constructible_v<T> T SharedValue<T>::get()
const
{ return *value; }

template<typename T>
requires std::is_default_constructible_v<T>
void SharedValue<T>::set(T &&valueIn)
{ *value = valueIn; }

template<typename T>
requires std::is_default_constructible_v<T> SharedValue<T>::SharedValue()
{
    value = std::make_shared<T>();
}

template<typename T>
requires std::is_default_constructible_v<T>
SharedValue<T>::SharedValue(T valueIn)
{
    value = std::make_shared<T>(valueIn);
}

template<typename T>
requires std::is_default_constructible_v<T> SharedValue<T>
&SharedValue<T>::operator=(const T &right)
{
    *value = right;
    return *this;
}

template<typename T>
requires std::is_default_constructible_v<T> SharedValue<T>
&SharedValue<T>::operator=(const SharedValue<T> &right)
{
    if (this == &right)
        return *this;

    value = right.value;
    return *this;
}
