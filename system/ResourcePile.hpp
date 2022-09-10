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

#pragma once

#include "define.hpp"
#include "Resource.hpp"

#include <unordered_map>
#include <vector>
#include <string>

NS_BEGIN

class Resources;

namespace detail
{

template<typename T>
struct ResourcePile
{
    using StorageT = Resource<T>;

//    explicit ResourcePile(Resources &res);

//    int loadPersistent();

    [[nodiscard]] StorageT get(const std::string &name) const;

    [[nodiscard]] StorageT get(const std::string &name, const std::filesystem::path &path);

    [[nodiscard]] StorageT load(const std::string &name, const std::filesystem::path &path);

//    [[nodiscard]] StorageT getDefault() const;

    void loadOne(const std::string &name, const std::filesystem::path &pathPrefix = "");

    unsigned int purgeUnusedFiles();

//    static const std::vector<std::string> allowedFileExtensions;
    std::unordered_map<std::string, StorageT> loadedAssets;
//    StorageT null;
//    Resources &resourceRef;
};

} // namespace detail

NS_END