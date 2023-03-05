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

//
// Created by sijh on 10.04.22.
//
#pragma once

#include "define.hpp"

#include "Resource.hpp"
#include "Color.hpp"
#include "Rect.hpp"

#include <filesystem>
#include <memory>

namespace PROJECT_NAMESPACE {

namespace video
{

class Image
{
public:
	Image();

	bool resize(int width, int height, bool noClear = false);

	void clear(color col = WHITE);

	void setPixel(uvec2d pos, color8 col);

	void setRectArea(irect area, color col);

	[[nodiscard]] int getWidth() const;

	[[nodiscard]] int getHeight() const;

	[[nodiscard]] int getChannels() const;

	[[nodiscard]] isize getSize() const;

	[[nodiscard]] color8 *getPixels();

	void free();

private:
	static void DeleteImage(color8 *);

	std::shared_ptr<color8> data;
	int width{-1}, height{-1}, channels{0};
};

}

template<>
Resource<video::Image> Load(const std::filesystem::path& path);

}
