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

#include <memory>
#include <vector>
#include <string>
#include <filesystem>

NS_BEGIN
class Resources;

template <typename T> class Resource;

template <typename T>
Resource<T> Default()
{
	return Resource<T>::defaultValue;
}

template <typename T> Resource<T> Load(const std::filesystem::path&);
template <typename T> Resource<T> Create();

template <typename T>
class Resource
{
	friend Resource<T> Load <T>(const std::filesystem::path& path);
	friend Resource<T> Create <T>();
	friend Resource<T> Default <T>();
public:
	Resource()
	{
		held = std::make_shared<T>();
	}

	Resource(nullptr_t)
	{
		held = defaultValue.held;
	}

	operator bool () const
	{
		return (held != defaultValue.held) && held;
	}

	operator std::weak_ptr<T> () const
	{
		return {held};
	}

	std::weak_ptr<T> ref () const
	{
		return {held};
	}

	T* operator -> () const
	{
		return held.get();
	}

	T* get () const
	{
		return held.get();
	}

	T& operator* () const
	{
		return *held;
	}

	auto useCount() const
	{
		return held.use_count();
	}

	static const std::vector<std::string> allowedExtensions;

protected:
	using ResourceT = Resource<T>;

	std::shared_ptr<T> held;
	static Resource<T> defaultValue;
};

template <typename T>
Resource<T> Resource<T>::defaultValue = Create<T>();
template <typename T>
const std::vector<std::string> Resource<T>::allowedExtensions;

template <typename T>
Resource<T> Create() { return {}; }

template <typename T>
Resource<T> Load(const std::filesystem::path&)
{
	WRAP_CONSTEXPR_ASSERTION("Load method not overridden for T.");
	return {};
}

//namespace detail
//{
//
//class Resource
//{
//public:
//    Resource() = default;
//
//    virtual ~Resource() = default;
//
//    virtual bool load(const std::filesystem::path &path) = 0;
//
//    virtual bool create() = 0;
//};
//
//} // namespace detail
//
//template<typename T>
//concept IsResource = std::is_base_of_v<detail::Resource, T> and std::is_default_constructible_v<T>;

NS_END