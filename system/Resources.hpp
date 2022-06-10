#pragma once

#include <filesystem>
#include <vector>
#include <map>

#include "Texture.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "MapInfo.hpp"

NS_BEGIN
class Resources;

template<typename T>
struct ResourcePile {
    explicit ResourcePile(Resources &res);

    int loadPersistent();

    bool loadOne(const std::string &path);

    [[nodiscard]] T &get(const std::string &name);

    [[nodiscard]] const T &get(const std::string &name) const;

private:
    static T makeDefault();

    static const char* persistentAssets[];
    Resources &resourceRef;
    std::map<std::string, T> loadedAssets;
    T null;
};

class Resources {
public:
    Resources();

    int loadPersistentAssets();

    ResourcePile<Texture> textures;
    ResourcePile<Shader> shaders;
    ResourcePile<Mesh> meshes;
    ResourcePile<MapInfo> maps;
    // ResourcePile<Sound> sounds;
    // ResourcePile<MapTemplate> maps;

    void addSearchPath(const std::filesystem::path &path);

    void clearSearchPaths();

    std::filesystem::path findFile(std::filesystem::path pathIn);

private:
    std::vector<std::filesystem::path> searchPaths;
};

template<typename T>
int ResourcePile<T>::loadPersistent() {
    null = makeDefault();
    int c = 0;
    for (int i = 0; i < ARRAY_SIZE(persistentAssets); i++) {
        c += loadOne(persistentAssets[i]);
    }
    return c;
}

template<typename T>
T &ResourcePile<T>::get(const std::string &name) {
    if (loadedAssets.find(name) == loadedAssets.end())
        if (!loadOne(name))
            return null;

    return loadedAssets[name];
}

template<typename T>
const T &ResourcePile<T>::get(const std::string &name) const {
    if (loadedAssets.find(name) == loadedAssets.end())
        return null;
    return loadedAssets[name];
}

template<typename T>
ResourcePile<T>::ResourcePile(Resources &res) : resourceRef(res) {}

NS_END