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
#include "Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "Util.hpp"

#include <cstring>

namespace PROJECT_NAMESPACE {

namespace video
{

Image::Image()
{ stbi_set_flip_vertically_on_load(true); }

int Image::getWidth() const
{
	if (!data)
		return -1;
	return width;
}

int Image::getHeight() const
{
	if (!data)
		return -1;
	return height;
}

int Image::getChannels() const
{
	if (!data)
		return -1;
	return channels;
}

color8 *Image::getPixels()
{
	if (!data)
		return nullptr;
	return data.get();
}

isize Image::getSize() const
{
	if (!data)
		return ::PROJECT_NAMESPACE::isize{-1, -1};
	return ::PROJECT_NAMESPACE::isize{width, height};
}

bool Image::resize(int widthIn, int heightIn, bool noClear)
{
	auto mem = (color8 *)std::malloc(widthIn * heightIn * sizeof(color8));
	if (mem) {
		width = widthIn;
		height = heightIn;
		channels = 4;
		data.reset(mem, DeleteImage);
		if (!noClear) {
			clear(WHITE);
		}
		return true;
	}
	return false;
}

void Image::clear(color col)
{
	if (!data)
		return;

	for (int i = 0; i < width * getHeight(); i++) {
		data.get()[i] = col;
	}
}

void Image::setPixel(uvec2d pos, color8 col)
{
	if (!data)
		return;

	data.get()[(pos[1] % height) * width + (pos[0] % width)] = col;
}

void Image::setRectArea(irect area, color col)
{
	if (!data)
		return;

	for (int x = area.position[0]; x < area.position[0] + area.size.w; x++) {
		for (int y = area.position[1]; y < area.position[1] + area.size.h; y++) {
			setPixel(isize{x, y}, col);
		}
	}
}

void Image::DeleteImage(color8 *ptr)
{ stbi_image_free(ptr); }

void Image::free()
{
	data.reset();
}

}

template<>
Resource<video::Image> Load(const std::filesystem::path &path)
{
	Resource<video::Image> r;

#ifdef LINUX
	const char* ptr = path.c_str();
#else
	auto string = path.u8string();
	const char *ptr = (const char *)string.c_str();
#endif

	int width, height, channels;

	auto *data = (color8 *)stbi_load(ptr, &width, &height, &channels, STBI_rgb_alpha);

	if (!data) {
		return {nullptr};
	}

	r->resize(width, height, true);

	auto *destination = r->getPixels();

	std::memcpy(destination, data, width * height * sizeof(color8));

	stbi_image_free(data);

//    log::Info("Loaded image ", path, " Dimensions: ", width, " x ",
//              std::to_string(height), " Channels: ", channels);

	return r;
}

}