#pragma once

#include <filesystem>
#include <unordered_map>
#include <vector>

#include "MapInfo.hpp"
#include "Mesh.hpp"
#include "Resource.hpp"
#include "Shader.hpp"
#include "Skin.hpp"
#include "Texture.hpp"

#include "ResourcePile.dpp"

NS_BEGIN

class Resources
{
public:
    Resources();

    int loadPersistentAssets();

    ResourcePile<Texture> textures;
    ResourcePile<Shader> shaders;
    ResourcePile<Mesh> meshes;
    ResourcePile<MapInfo> maps;
    ResourcePile<Skin> skins;
    // ResourcePile<Sound> sounds;

    void addSearchPath(const std::filesystem::path &path);

    void clearSearchPaths();

    std::filesystem::path findFile(const std::filesystem::path &pathIn,
                                   const std::filesystem::path &pathPrefix = "");

private:
    std::vector<std::filesystem::path> searchPaths;
};

NS_END