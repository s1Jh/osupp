#include "Resources.hpp"

#include "MapLoaders.hpp"
#include "MeshLoaders.hpp"
#include "Util.hpp"

NS_BEGIN

namespace detail
{

template<>
const char *ResourcePile<Texture>::persistentAssets[] = {
    NOTE_BASE_SPRITE, NOTE_OVERLAY_SPRITE,
    NOTE_UNDERLAY_SPRITE, APPROACH_CIRCLE_SPRITE,
    SLIDER_HEAD_SPRITE, SLIDER_TAIL_SPRITE,
    SLIDER_HEAD_REPEAT_SPRITE, SLIDER_TAIL_REPEAT_SPRITE,
    SLIDER_BODY_SPRITE, SPINNER_SPRITE,
    SPINNER_CENTER_SPRITE
};

template<>
const char *ResourcePile<Shader>::persistentAssets[] = {SLIDER_SHADER};

template<> const char *ResourcePile<Mesh>::persistentAssets[] = {""};

template<> const char *ResourcePile<MapInfo>::persistentAssets[] = {""};

template<> const char *ResourcePile<Skin>::persistentAssets[] = {"default"};

} // namespace detail

int Resources::loadPersistentAssets()
{
    int success = 0;

#define RESOURCE_POOL(_Type, _Name, ...) success += (_Name).loadPersistent();
    RESOURCE_POOLS
#undef RESOURCE_POOL

    return success;
}

void Resources::addSearchPath(const std::filesystem::path &path)
{
    log::info("Adding search path ", path.string());

    if (std::filesystem::exists(path)) {
        searchPaths.push_back(path);
        return;
    }
    log::warning("Failed to add search path, path doesn't exist");
}

void Resources::clearSearchPaths()
{ searchPaths.clear(); }

std::filesystem::path
Resources::findFile(const std::filesystem::path &pathIn,
                    const std::filesystem::path &pathPrefix) const
{
    for (auto &path: searchPaths) {
        auto concatPath = path / pathPrefix / pathIn.parent_path();
        if (std::filesystem::is_directory(concatPath))
            for (auto &fileInPath:
                std::filesystem::recursive_directory_iterator(concatPath)) {
                auto fileName = fileInPath.path().stem().string();

                if (fileName == pathIn.stem().string()) {
                    return fileInPath.path();
                }
            }
    }

    return pathIn;
}

Resources::Resources()
    :
#define RESOURCE_POOL(_Type, _Name, ...) _Name(*this),
    RESOURCE_POOLS
#undef RESOURCE_POOL
    searchPaths()
{}

NS_END