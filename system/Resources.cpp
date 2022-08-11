/*******************************************************************************
 * Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

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

template<> const std::vector<std::string> detail::ResourcePile<SoundSample>::allowedFileExtensions =
	{".wav", ".mp3", ".ogg"};

template<> const std::vector<std::string> detail::ResourcePile<SoundStream>::allowedFileExtensions =
	{".wav", ".mp3", ".ogg"};

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
                    const std::vector<std::string> &allowedExts,
					bool caseSensitive) const
{
	auto searchedFile = pathIn.stem().string();

	if (!caseSensitive)
		std::transform(searchedFile.begin(), searchedFile.end(), searchedFile.begin(), ::tolower);

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

				if (!caseSensitive)
					std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);

                if (!allowedExts.empty()) {
                    auto ext = thisPath.extension().string();
                    if (std::find(allowedExts.begin(), allowedExts.end(), ext) == allowedExts.end())
                        continue;
                }

                if (fileName == searchedFile) {
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