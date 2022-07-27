#pragma once

#include "define.hpp"
#include "Resource.hpp"

#include <unordered_map>
#include <vector>
#include <string>

NS_BEGIN

class Resources;

namespace detail
{

template<typename T> requires IsResource<T>
struct ResourcePile
{
    using StorageT = std::shared_ptr<T>;

    explicit ResourcePile(Resources &res);

    int loadPersistent();

    [[nodiscard]] StorageT get(const std::string &name) const;

    [[nodiscard]] StorageT get(const std::string &name, const std::filesystem::path &path);

    [[nodiscard]] StorageT load(const std::string &name, const std::filesystem::path &path);

    [[nodiscard]] StorageT getDefault() const;

    void loadOne(const std::string &name, const std::filesystem::path &pathPrefix = "");

    unsigned int purgeUnusedFiles();

    static const std::vector<std::string> allowedFileExtensions;
    std::unordered_map<std::string, StorageT> loadedAssets;
    StorageT null;
    Resources &resourceRef;
};

} // namespace detail

NS_END