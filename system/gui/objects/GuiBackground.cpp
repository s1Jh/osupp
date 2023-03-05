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

#include "GuiBackground.hpp"

#include "GuiRoot.hpp"

#include "MatrixMath.hpp"
#include "Vector2Math.hpp"
#include "Interp.hpp"
#include "Functions.hpp"
#include "Constraint.hpp"

namespace PROJECT_NAMESPACE {

namespace gui
{

bool Background::create(const df2 &df)
{
    return true;
}
bool Background::update(Root &root)
{
    if (parallax) {
        auto mousePos = root.getIO().mouse.position();

        float parallaxFactor = math::Min(math::Mag(mousePos), parallaxFactor);

        fvec2d parallaxCenter = math::BiLerp(fvec2d{0, 0}, mousePos, math::SmoothStep(parallaxFactor));

        auto scale = 1.0f - parallaxStrength;
        parallaxMatrix = math::MakeScaleMatrix(fvec2d{scale, scale}) * math::MakeTranslationMatrix(parallaxCenter);

    } else {
        parallaxMatrix = MAT3_NO_TRANSFORM<float>;
    }
    return true;
}

bool Background::draw(video::LambdaRender &render)
{
    video::VisualAppearance appearance;
    appearance.flags = video::AppearanceFlags::IGNORE_CAMERA;
    if (imageBackground) {
        appearance.texture = imageBackground;
    } else {
        appearance.fillColor = solidBackground;
    }

    render.draw(DrawRect {
        SCREEN_RECT<float>,
        appearance,
        MAT3_NO_TRANSFORM<float>
    }, 0);
    return true;
}

void Background::configureParallax(float strength)
{
    parallax = true;
    parallaxStrength = strength;
}

void Background::configureParallax(bool newState)
{
    parallax = newState;
}

void Background::setImageBackground(const Resource<video::Texture> &imageBackground)
{
    if (!imageBackground->uploaded()) {
        imageBackground->upload();
    }
    Background::imageBackground = imageBackground;
}

void Background::setSolidBackground(const color &solidBackground)
{
    Background::solidBackground = solidBackground;
}

}

}
