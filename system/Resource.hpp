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

template <typename T> Resource<T> Default();
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
		held = Default<T>().held;
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
	Resource(nullptr_t, nullptr_t)
	{
		held = nullptr;
	}

	using ResourceT = Resource<T>;

	std::shared_ptr<T> held;
	static Resource<T> defaultValue;
};

template <typename T>
Resource<T> Resource<T>::defaultValue{ nullptr, nullptr };
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

template <typename T>
Resource<T> Default()
{
	auto& d = Resource<T>::defaultValue;
	if (!d.held) {
		// HACK: To prevent infinite recursion, we make sure the pointer
		// is valid, so that subsequent default checks don't try to create
		// it as well.
		d.held = std::make_shared<T>();
		d = Create<T>();
	}
	return d;
}

NS_END