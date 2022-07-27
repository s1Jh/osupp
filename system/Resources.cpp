#include "Resources.hpp"

#include "MapLoaders.hpp"
#include "MeshLoaders.hpp"
#include "Util.hpp"

NS_BEGIN

template<> const std::vector<std::string> detail::ResourcePile<Texture>::allowedFileExtensions = {".png", ".jpg"};

template<> const std::vector<std::string> detail::ResourcePile<Shader>::allowedFileExtensions = {".shader"};

template<> const std::vector<std::string> detail::ResourcePile<Mesh>::allowedFileExtensions = {".obj"};

template<> const std::vector<std::string> detail::ResourcePile<MapInfo>::allowedFileExtensions = {".map", ".osu"};

template<> const std::vector<std::string> detail::ResourcePile<Skin>::allowedFileExtensions = {".skin"};

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
                    const std::filesystem::path &pathPrefix,
                    const std::vector<std::string> &allowedExts) const
{
    for (auto &path: searchPaths) {
        auto concatPath = path / pathPrefix / pathIn.parent_path();
        if (std::filesystem::is_directory(concatPath))
            for (auto &fileInPath:
                std::filesystem::recursive_directory_iterator(concatPath)) {
                if (std::filesystem::is_directory(fileInPath)) {
                    continue;
                }
                auto thisPath = fileInPath.path();
                auto fileName = thisPath.stem().string();

                if (!allowedExts.empty()) {
                    auto ext = thisPath.extension().string();
                    if (std::find(allowedExts.begin(), allowedExts.end(), ext) == allowedExts.end())
                        continue;
                }

                if (fileName == pathIn.stem().string()) {
                    return fileInPath.path();
                }
            }
    }

    return pathIn;
}

std::vector<std::filesystem::path>
Resources::findFiles(const std::filesystem::path &pathPrefix,
                     const std::vector<std::string> &allowedExts) const
{
    std::vector<std::filesystem::path> ret;

    for (auto &path: searchPaths) {
        auto concatPath = path / pathPrefix;
        if (std::filesystem::is_directory(concatPath))
            for (auto &fileInPath:
                std::filesystem::recursive_directory_iterator(concatPath)) {
                if (fileInPath.is_directory()) {
                    continue;
                }
                auto thisPath = fileInPath.path();

                if (!allowedExts.empty()) {
                    auto ext = thisPath.extension().string();
                    if (std::find(allowedExts.begin(), allowedExts.end(), ext) == allowedExts.end())
                        continue;
                }

                ret.push_back(thisPath);
            }
    }

    return ret;
}

Resources::Resources()
    :
#define RESOURCE_POOL(_Type, _Name, ...) _Name(*this),
    RESOURCE_POOLS
#undef RESOURCE_POOL
    searchPaths()
{}

unsigned int Resources::purgeUnusedFiles()
{
    unsigned int total = 0;

#define RESOURCE_POOL(_Type, _Name, ...) total += _Name.purgeUnusedFiles();
    RESOURCE_POOLS
#undef RESOURCE_POOL

    return total;
}

NS_END