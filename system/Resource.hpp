#pragma once

#include "define.hpp"

#include <string>

NS_BEGIN
class Resources;

namespace detail
{
class Resource;
}

template<typename T>
concept IsResource = std::is_base_of_v<detail::Resource, T> and
    std::is_default_constructible_v<T>;

template<typename T> requires IsResource<T>
struct ResourcePile;

namespace detail
{
class Resource
{
public:
    Resource() = default;

    virtual ~Resource() = default;

    virtual bool load(const std::string &path, Resources *res) = 0;

    virtual bool create(Resources *res) = 0;
};
} // namespace detail

NS_END