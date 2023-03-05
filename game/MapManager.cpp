//=*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*=
// Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
//                                      =*=
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//                                      =*=
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//                                      =*=
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.                            =*=
//=*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*=
#include "MapManager.hpp"

#include "Tasks.hpp"

#include <algorithm>

namespace PROJECT_NAMESPACE {

int MapManager::load(const files::MultiDirectorySearch &source)
{
	auto filesToLoad = source.findAll("songs", Resource<MapInfo>::allowedExtensions);

	auto fileCount = static_cast<int>(filesToLoad.size());

	for (auto &file : filesToLoad) {
		results.push_back(tasks::MakeSimple(LoadTask<MapInfo>(), std::move(file)));
	}

	return fileCount;
}

int MapManager::update()
{
	for (const auto &task : results) {
		if (!task.isComplete()) {
			continue;
		}

		auto result = task.getResult();
		if (!result) {
			log::Warning("Load task marked complete had incomplete result.");
			continue;
		}

		auto &value = result.value();
		if (bool(value)) {
			maps.push_back(std::move(value));
		}
	}

	auto startingSize = (int)results.size();

	results.erase(
		std::remove_if(results.begin(), results.end(), [](const auto &task)
		{ return task.isComplete(); }),
		results.end()
	);

	return startingSize - (int)results.size();
}

bool MapManager::isLoading() const
{
	return !results.empty();
}

size_t MapManager::size() const
{
	return maps.size();
}

MapManager::MapStorageType::const_iterator MapManager::cbegin() const
{
	return maps.cbegin();
}

MapManager::MapStorageType::const_iterator MapManager::cend() const
{
	return maps.cend();
}

Resource<MapInfo> MapManager::at(unsigned int i)
{
	return maps[i % maps.size()];
}

void MapManager::clear()
{
    maps.clear();
    results.clear();
}

size_t MapManager::remaining() const
{
    return results.size();
}

}