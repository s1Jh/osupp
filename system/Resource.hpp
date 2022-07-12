#pragma once

#include "define.hpp"

#include <string>
#include <filesystem>

NS_BEGIN
class Resources;

namespace detail
{

class Resource
{
public:
    Resource() = default;

    virtual ~Resource() = default;

    virtual bool load(const std::filesystem::path &path) = 0;

    virtual bool create() = 0;
};

} // namespace detail

template<typename T>
concept IsResource = std::is_base_of_v<detail::Resource, T> and std::is_default_constructible_v<T>;

NS_END