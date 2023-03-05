// Copyright (c) 2023 sijh (s1Jh.199[at]gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//
// Created by sijh on 16.02.23.
//
#pragma once

#include "define.hpp"

#include "GuiElement.hpp"

#include "Texture.hpp"

namespace PROJECT_NAMESPACE {

namespace gui
{

class Background : public detail::IElement
{
public:
    void configureParallax(float strength);
    void configureParallax(bool);

    void setImageBackground(const Resource<video::Texture> &imageBackground);
    void setSolidBackground(const color &solidBackground);

protected:
    bool create(const df2 &df) override;
    bool update(Root &root) override;
    bool draw(video::LambdaRender &render) override;

private:
    Resource<video::Texture> imageBackground{nullptr};
    color solidBackground{BLACK};
    Mat3f parallaxMatrix;
    bool parallax{true};
    float parallaxStrength{0.2f};
};

}

}
