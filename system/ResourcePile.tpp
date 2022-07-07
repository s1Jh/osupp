#pragma once

#include "Resource.hpp"
#include "ResourcePile.dpp"
#include "Resources.hpp"

NS_BEGIN

template<typename T>
requires IsResource<T>
int ResourcePile<T>::loadPersistent()
{
    null->create(&resourceRef);

    int c = 0;
    for (unsigned int i = 0; i < ARRAY_SIZE(persistentAssets); i++) {
        if (get(persistentAssets[i]))
            c++;
    }
    return c;
}

template<typename T>
requires IsResource<T>
bool ResourcePile<T>::loadOne(const std::string &path)
{
    StorageT object = std::make_shared<T>();
    if (!object->load(resourceRef.findFile(path), &resourceRef)) {
        log::error("Failed to load ", typeid(T).name(), ' ', path);
        return false;
    }
    loadedAssets[path] = object;
    return true;
}

template<typename T>
requires IsResource<T>
typename ResourcePile<T>::StorageT
ResourcePile<T>::get(const std::string &name) const
{
    if (loadedAssets.find(name) == loadedAssets.end())
        loadedAssets[name] = null;

    return loadedAssets[name];
}

template<typename T>
requires IsResource<T>
typename ResourcePile<T>::StorageT
ResourcePile<T>::get(const std::string &name)
{
    if (loadedAssets.find(name) == loadedAssets.end())
        if (!loadOne(name))
            loadedAssets[name] = null;
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
    : resourceRef(res)
{
    null = std::make_shared<T>();
}

NS_END