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

#include "Texture.hpp"

#include "Math.hpp"
#include "Util.hpp"
#include "GraphicsContext.hpp"
#include "GL.hpp"

NS_BEGIN

namespace video
{

Texture::Texture() : channels(-1), pixelSize({-1, -1})
{
    clip = MAT3_NO_TRANSFORM<float>;
}

void Texture::use(unsigned int index) const
{
	if (index > 31) {
		log::warning("Tried to set texture beyond index");
		return;
	}
	if (uploaded()) {
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, getGLData());
	}
}

void Texture::unbind(unsigned int index)
{
	if (index > 31) {
		log::warning("Tried to set texture beyond index");
		return;
	}

	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::getWidth() const
{ return pixelSize.w; }

int Texture::getHeight() const
{ return pixelSize.h; }

int Texture::getChannels() const
{ return channels; }


void Texture::setImage(Image &imgIn)
{
	img = imgIn;
}

isize Texture::getSize() const
{ return pixelSize; }

void Texture::setClipArea(const frect &rect)
{
	clip = math::MakeScaleMatrix<float>(rect.size) *
		math::MakeTranslationMatrix<float>(rect.position);
}

const Mat3f &Texture::getUVTransform() const
{ return clip; }

std::optional<unsigned int> Texture::createData()
{
	unsigned int id;

	if (img.getPixels() == nullptr) {
		return {};
	}

	auto pixels = img.getPixels();

	pixelSize = img.getSize();
	channels = img.getChannels();

	glGenTextures(1, &id);
	CheckGLh("Generated texture");
	glBindTexture(GL_TEXTURE_2D, id);
	CheckGLh("Bound texture");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	CheckGLh("Set texture params");

	// you take GL_RGB and then add channels - 3 (those are the R, G and B
	// channels)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB + (img.getChannels() - 3),
				 img.getWidth(), img.getHeight(), 0,
				 GL_RGB + (img.getChannels() - 3), GL_UNSIGNED_BYTE,
				 (uint8_t *)pixels);
	CheckGLh("Uploaded texture data");
	glGenerateMipmap(GL_TEXTURE_2D);
	CheckGLh("Generated mipmaps");
	glBindTexture(GL_TEXTURE_2D, 0);

	img.free();
	return id;
}

void Texture::deleteData(const unsigned int &repr)
{
	glDeleteTextures(1, &repr);
}

Texture::Texture(unsigned int existing) : channels(-1), pixelSize({-1, -1})
{
    overrideData(existing);
    clip = MAT3_NO_TRANSFORM<float>;
}

}


template<>
Resource<video::Texture> Load(const std::filesystem::path &location)
{
	Resource<video::Texture> tex;
	Resource<video::Image> texImg = Load<video::Image>(location);

	if (!texImg) {
		return {nullptr};
	}

	tex.held->setImage(*texImg);

	return tex;
}

template<>
Resource<video::Texture> Create()
{
	video::Image image;
	image.resize(128, 128);
	image.setRectArea({{64, 64}, {0, 0}}, BLACK);
	image.setRectArea({{64, 64}, {0, 64}}, PINK);
	image.setRectArea({{64, 64}, {64, 0}}, PINK);
	image.setRectArea({{64, 64}, {64, 64}}, BLACK);

	Resource<video::Texture> tex;
	tex->setImage(image);

	return tex;
}

NS_END