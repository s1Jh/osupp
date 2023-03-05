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
#pragma once

#include "define.hpp"

#include <filesystem>
#include <vector>
#include <optional>

namespace PROJECT_NAMESPACE {

namespace files
{

std::vector<std::filesystem::path> FindAll(const std::filesystem::path &pathPrefix,
										   const std::vector<std::string> &allowedExts = {});

std::filesystem::path Get(const std::filesystem::path &pathIn,
						  const std::filesystem::path &pathPrefix,
						  const std::vector<std::string> &allowedExts = {},
						  bool caseSensitive = false);

std::optional<std::filesystem::path> Search(const std::filesystem::path &pathIn,
											const std::filesystem::path &pathPrefix,
											const std::vector<std::string> &allowedExts = {},
											bool caseSensitive = false);

class MultiDirectorySearch
{
public:
	using PathCollectionType = std::vector<std::filesystem::path>;
	MultiDirectorySearch();
	explicit MultiDirectorySearch(PathCollectionType initPaths);
	explicit MultiDirectorySearch(PathCollectionType &&initPaths);

	bool addPath(const std::filesystem::path &pathIn);

	[[nodiscard]] PathCollectionType findAll(const std::filesystem::path &path,
							   const std::vector<std::string> &allowedExts = {}) const;

	[[nodiscard]] std::filesystem::path find(const std::filesystem::path &pathIn,
							   const std::vector<std::string> &allowedExts = {},
							   bool caseSensitive = false) const;

	inline PathCollectionType::iterator begin() { return searchPaths.begin(); }
	inline PathCollectionType::iterator end() { return searchPaths.end(); }

	inline PathCollectionType::const_iterator cbegin() { return searchPaths.cbegin(); }
	inline PathCollectionType::const_iterator cend() { return searchPaths.cend(); }

private:
	PathCollectionType searchPaths;
};

}

}
