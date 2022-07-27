#pragma once

#include "define.hpp"

#define USER_RESOURCE_INCLUDES
#include "config.hpp"

#include <filesystem>
#include <unordered_map>
#include <vector>

#include "Mesh.hpp"
#include "Resource.hpp"
#include "ResourcePile.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#ifndef USER_RESOURCE_POOLS
#define USER_RESOURCE_POOLS
#endif

#define RESOURCE_POOL_FIRST \
    RESOURCE_POOL(Texture, textures)

#define RESOURCE_POOL_CENTERS \
    RESOURCE_POOL(Shader, shaders) \
    USER_RESOURCE_POOLS

#define RESOURCE_POOL_LAST  \
    RESOURCE_POOL(Mesh, meshes)

#define RESOURCE_POOLS \
    RESOURCE_POOL_FIRST\
    RESOURCE_POOL_CENTERS \
    RESOURCE_POOL_LAST

NS_BEGIN

class Resources
{
public:
    Resources();

    int loadPersistentAssets();

    template<typename T>
    requires IsResource<T>
    std::shared_ptr<T> getDefault() const;

    template<typename T>
    requires IsResource<T>
    std::shared_ptr<T> get(const std::string &name) const;

    template<typename T>
    requires IsResource<T>
    std::shared_ptr<T> get(const std::string &name, const std::filesystem::path &path = "");

    template<typename T>
    requires IsResource<T>
    std::shared_ptr<T> load(const std::string &name, const std::filesystem::path &path = "");

    template<typename T>
    requires IsResource<T>
    void loadDirectory(const std::filesystem::path &path);

    template<typename T>
    requires IsResource<T>
    void getDirectory(const std::filesystem::path &path, std::vector<std::shared_ptr<T>> &returnAssets);

    void addSearchPath(const std::filesystem::path &path);

    void clearSearchPaths();

    std::filesystem::path findFile(const std::filesystem::path &pathIn,
                                   const std::filesystem::path &pathPrefix = "",
                                   const std::vector<std::string> &allowedExts = {}) const;

    std::vector<std::filesystem::path> findFiles(const std::filesystem::path &pathPrefix,
                                                 const std::vector<std::string> &allowedExts = {}) const;

    unsigned int purgeUnusedFiles();

private:
#define RESOURCE_POOL(_Type, _Name, ...) detail::ResourcePile<_Type> _Name;
    RESOURCE_POOLS
#undef RESOURCE_POOL

    std::vector<std::filesystem::path> searchPaths;
};

namespace detail
{

template<typename T>
requires IsResource<T>
int ResourcePile<T>::loadPersistent()
{
    null->create();
    return 0;
}

template<typename T>
requires IsResource<T>
void ResourcePile<T>::loadOne(const std::string &name, const std::filesystem::path &path)
{
    // copy the null object
    StorageT object = std::make_shared<T>(*null);
    loadedAssets[name] = object;
    if (!object->load(path)) {
        log::error("Failed to load ", typeid(T).name(), ' ', path);
    }
}

template<typename T>
requires IsResource<T>
typename ResourcePile<T>::StorageT
ResourcePile<T>::get(const std::string &name) const
{
    if (loadedAssets.find(name) == loadedAssets.end())
        return null;

    return loadedAssets.at(name);
}

template<typename T>
requires IsResource<T>
typename ResourcePile<T>::StorageT
ResourcePile<T>::get(const std::string &name, const std::filesystem::path &path)
{
    if (loadedAssets.find(name) == loadedAssets.end())
        loadOne(name, path);

    return loadedAssets[name];
}

template<typename T>
requires IsResource<T>
typename ResourcePile<T>::StorageT
ResourcePile<T>::load(const std::string &name, const std::filesystem::path &path)
{
    loadOne(name, path);
    return loadedAssets[name];
}

template<typename T>
requires IsResource<T>
typename ResourcePile<T>::StorageT ResourcePile<T>::getDefault() const
{
    return null;
}

template<typename T>
requires IsResource<T> ResourcePile<T>::ResourcePile(Resources &res)
    :
    resourceRef(res)
{
    null = std::make_shared<T>();
}

template<typename T>
requires IsResource<T>
unsigned int ResourcePile<T>::purgeUnusedFiles()
{
    std::erase_if(loadedAssets, [](const auto &asset)
    { return asset.second.use_count() <= 1; });
}

} // namespace detail

template<typename T>
requires IsResource<T>
std::shared_ptr<T> Resources::getDefault() const
{
#define RESOURCE_POOL(_Type, _Name, ...) if constexpr (std::is_same_v<T, _Type>) return (_Name).null;
    RESOURCE_POOL_FIRST
#undef RESOURCE_POOL
#define RESOURCE_POOL(_Type, _Name, ...) else if constexpr (std::is_same_v<T, _Type>) return (_Name).null;
    RESOURCE_POOL_CENTERS RESOURCE_POOL_LAST
#undef RESOURCE_POOL
    else {
        WRAP_CONSTEXPR_ASSERTION("This type is not managed by the resource manager");
    }
}

template<typename T>
requires IsResource<T>
std::shared_ptr<T> Resources::get(const std::string &name) const
{
#define RESOURCE_POOL(_Type, _Name, ...) if constexpr (std::is_same_v<T, _Type>) return (_Name).get(name);
    RESOURCE_POOL_FIRST
#undef RESOURCE_POOL
#define RESOURCE_POOL(_Type, _Name, ...) else if constexpr (std::is_same_v<T, _Type>) return (_Name).get(name);
    RESOURCE_POOL_CENTERS RESOURCE_POOL_LAST
#undef RESOURCE_POOL
    else {
        WRAP_CONSTEXPR_ASSERTION("This type is not managed by the resource manager");
    }
}

template<typename T>
requires IsResource<T>
std::shared_ptr<T> Resources::get(const std::string &name, const std::filesystem::path &path)
{
    auto fullPath = findFile(name, path, detail::ResourcePile<T>::allowedFileExtensions);
    if (fullPath.empty())
        return getDefault<T>();

#define RESOURCE_POOL(_Type, _Name, ...) if constexpr (std::is_same_v<T, _Type>) return (_Name).get(name, fullPath);
    RESOURCE_POOL_FIRST
#undef RESOURCE_POOL
#define RESOURCE_POOL(_Type, _Name, ...) else if constexpr (std::is_same_v<T, _Type>) return (_Name).get(name, fullPath);
    RESOURCE_POOL_CENTERS RESOURCE_POOL_LAST
#undef RESOURCE_POOL
    else {
        WRAP_CONSTEXPR_ASSERTION("This type is not managed by the resource manager");
    }
}

template<typename T>
requires IsResource<T>
std::shared_ptr<T> Resources::load(const std::string &name, const std::filesystem::path &path)
{
    auto fullPath = findFile(name, path, detail::ResourcePile<T>::allowedFileExtensions);
    if (fullPath.empty())
        return getDefault<T>();

#define RESOURCE_POOL(_Type, _Name, ...) if constexpr (std::is_same_v<T, _Type>) return (_Name).load(name, fullPath);
    RESOURCE_POOL_FIRST
#undef RESOURCE_POOL
#define RESOURCE_POOL(_Type, _Name, ...) else if constexpr (std::is_same_v<T, _Type>) return (_Name).load(name, fullPath);
    RESOURCE_POOL_CENTERS RESOURCE_POOL_LAST
#undef RESOURCE_POOL
    else {
        WRAP_CONSTEXPR_ASSERTION("This type is not managed by the resource manager");
    }
}
template<typename T>
requires IsResource<T>
void Resources::loadDirectory(const std::filesystem::path &path)
{
    auto files = findFiles(path, detail::ResourcePile<T>::allowedFileExtensions);

    for (const auto &file: files)
        load<T>(file);
}

template<typename T>
requires IsResource<T>
void Resources::getDirectory(const std::filesystem::path &path, std::vector<std::shared_ptr<T>> &returnAssets)
{
    auto files = findFiles(path, detail::ResourcePile<T>::allowedFileExtensions);
    for (const auto &file: files)
        returnAssets.push_back(get<T>(file));
}

NS_END