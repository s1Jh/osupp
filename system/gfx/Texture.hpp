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

#include "Image.hpp"
#include "Resource.hpp"
#include "Vec2.hpp"
#include "define.hpp"

#include <memory>
#include <filesystem>

NS_BEGIN

class Texture: public detail::Resource
{
public:
    Texture();

    bool load(const std::filesystem::path &location) override;

    bool create() override;

    bool setImage(Image &img);

    [[nodiscard]] int getWidth() const;

    [[nodiscard]] int getHeight() const;

    [[nodiscard]] isize getSize() const;

    [[nodiscard]] int getChannels() const;

    void use(unsigned int index) const;

    void setClipArea(const frect &rect);
    [[nodiscard]] const Mat3f &getUVTransform() const;

    static void unbind(unsigned int index);

    [[nodiscard]] unsigned int getID() const;

private:
    static void GLTexDeleter(unsigned int *ptr);

    Mat3f clip;
    std::shared_ptr<unsigned int> glTexture;
    Image *img;
    uint8_t channels;
    isize pixelSize;
};

using TextureP = std::shared_ptr<Texture>;

NS_END