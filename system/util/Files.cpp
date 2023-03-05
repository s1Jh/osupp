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
#include "Files.hpp"

#include <memory>
#include <algorithm>
#include <utility>

namespace PROJECT_NAMESPACE {

namespace files
{

std::optional<std::filesystem::path> Search(const std::filesystem::path &pathIn,
											const std::filesystem::path &pathPrefix,
											const std::vector<std::string> &allowedExts,
											bool caseSensitive)
{
	auto searchedFile = pathIn.stem().string();

	if (!caseSensitive) {
		std::transform(searchedFile.begin(), searchedFile.end(), searchedFile.begin(), ::tolower);
	}

	auto concatPath = pathPrefix / pathIn.parent_path();
	if (!std::filesystem::is_directory(concatPath)) {
		return {};
	}

	for (auto &fileInPath :
		std::filesystem::recursive_directory_iterator(concatPath)) {
		if (std::filesystem::is_directory(fileInPath)) {
			continue;
		}
		const auto &thisPath = fileInPath.path();
		auto fileName = thisPath.stem().string();

		if (!caseSensitive) {
			std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
		}

		if (!allowedExts.empty()) {
			auto ext = thisPath.extension().string();
			if (std::find(allowedExts.begin(), allowedExts.end(), ext) == allowedExts.end()) {
				continue;
			}
		}

		if (fileName == searchedFile) {
			return fileInPath.path();
		}
	}

	return {};
}

std::vector<std::filesystem::path> FindAll(const std::filesystem::path &pathPrefix,
										   const std::vector<std::string> &allowedExts)
{
	std::vector<std::filesystem::path> ret;

	if (!std::filesystem::is_directory(pathPrefix)) {
		return ret;
	}

	for (auto &fileInPath :
		std::filesystem::recursive_directory_iterator(pathPrefix)) {
		if (fileInPath.is_directory()) {
			continue;
		}
		const auto &thisPath = fileInPath.path();

		if (!allowedExts.empty()) {
			auto ext = thisPath.extension().string();
			if (std::find(allowedExts.begin(), allowedExts.end(), ext) == allowedExts.end()) {
				continue;
			}
		}

		ret.push_back(thisPath);
	}

	return ret;
}
std::filesystem::path Get(const std::filesystem::path &pathIn,
						  const std::filesystem::path &pathPrefix,
						  const std::vector<std::string> &allowedExts,
						  bool caseSensitive)
{
	auto result = Search(pathIn, pathPrefix, allowedExts, caseSensitive);

	if (result) {
		return *result;
	}
	return pathIn;
}

MultiDirectorySearch::MultiDirectorySearch(MultiDirectorySearch::PathCollectionType initPaths)
	:
	searchPaths(std::move(initPaths))
{
	searchPaths.insert(searchPaths.begin(), std::filesystem::current_path());
}

MultiDirectorySearch::MultiDirectorySearch(MultiDirectorySearch::PathCollectionType &&initPaths)
	:
	searchPaths(std::move(initPaths))
{
	searchPaths.insert(searchPaths.begin(), std::filesystem::current_path());
}

bool MultiDirectorySearch::addPath(const std::filesystem::path &pathIn)
{
	if (std::filesystem::is_directory(pathIn)) {
		searchPaths.push_back(pathIn);
		return true;
	}
	return false;
}

MultiDirectorySearch::PathCollectionType MultiDirectorySearch::findAll(const std::filesystem::path &path,
																	   const std::vector<std::string> &allowedExts) const
{
	PathCollectionType ret;
	for (const auto &searchPath : searchPaths) {
		auto concatPath = searchPath / path;
		PathCollectionType results = FindAll(concatPath, allowedExts);
		ret.insert(ret.end(), results.begin(), results.end());
	}
	return ret;
}
std::filesystem::path MultiDirectorySearch::find(const std::filesystem::path &pathIn,
												 const std::vector<std::string> &allowedExts,
												 bool caseSensitive) const
{
	for (const auto &searchPath : searchPaths) {
		auto result = Search(pathIn, searchPath, allowedExts, caseSensitive);
		if (result) {
			return *result;
		}
	}
	return pathIn;
}

MultiDirectorySearch::MultiDirectorySearch()
{
	searchPaths.insert(searchPaths.begin(), std::filesystem::current_path());
}

}

}
