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

#include "Camera.hpp"

#include "Math.hpp"

namespace PROJECT_NAMESPACE {

Camera2D::Camera2D()
    : aspectRatio(1.f), position({0.f, 0.f}), rotation(0.f),
      viewScale({1.f, 1.f})
{
    recalculateMatrix();
}

void Camera2D::recalculateMatrix()
{
    cameraMatrix = math::MakeTranslationMatrix(position) *
        math::MakeRotationMatrix<float>(rotation) *
        math::MakeScaleMatrix(fvec2d{aspectRatio, 1.f} / viewScale);
}

const Mat3f &Camera2D::getMatrix() const
{ return cameraMatrix; }

void Camera2D::setAspectRatio(float ratio)
{ aspectRatio = ratio; }

float Camera2D::getAspectRatio() const
{ return aspectRatio; }

void Camera2D::rotate(float amount)
{ rotation += amount; }

void Camera2D::setRotation(float in)
{ rotation = in; }

void Camera2D::scale(const fvec2d &amount)
{ viewScale += amount; }

void Camera2D::setScale(const fvec2d &in)
{ viewScale = in; }

void Camera2D::translate(const fvec2d &amount)
{
    position += math::Rotate(amount, rotation);
}

void Camera2D::setPosition(const fvec2d &in)
{ position = in; }

}