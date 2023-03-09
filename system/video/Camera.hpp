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

#include "Matrix.hpp"
#include "Vector.hpp"

namespace PROJECT_NAMESPACE {

class Camera2D
{
public:
    Camera2D();

    enum class Direction
    {
//        UP, DOWN, LEFT, RIGHT, OUT, IN
    };

    void setPosition(const fvec2d &in);

    void translate(const fvec2d &amount);

    void setScale(const fvec2d &in);

    void scale(const fvec2d &amount);

    void setRotation(float in);

    void rotate(float amount);

    void setAspectRatio(float ratio);

    [[nodiscard]] float getAspectRatio() const;

    [[nodiscard]] const Mat3f &getMatrix() const;

    void recalculateMatrix();

private:
    float aspectRatio;
    fvec2d position;
    float rotation;
    fvec2d viewScale;
    Mat3f cameraMatrix;
};

}