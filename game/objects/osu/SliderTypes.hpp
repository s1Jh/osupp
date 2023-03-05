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

#include "Curve.hpp"
#include "Line.hpp"
#include "Vector.hpp"

#include <list>

namespace PROJECT_NAMESPACE {

struct SliderNode: public math::BaseCurveNode
{
    inline SliderNode(fvec2d _position, bool _bonus)
        : position(_position), bonus(_bonus)
    {}

    [[nodiscard]] inline fvec2d getPosition() const override
    { return position; }

    fvec2d position = {0.f, 0.f};
    bool bonus = false;
};

struct ActiveSliderNode: public math::BaseCurveNode
{
    inline explicit ActiveSliderNode(fvec2d _position)
        : position(_position)
    {}

    [[nodiscard]] inline fvec2d getPosition() const override
    { return position; }

    fvec2d position = {0.f, 0.f};
    bool bonus = false;
    float length = 0.0;
};

using SliderPathT = std::list<SliderNode>;
using ActiveSliderPathT = std::list<ActiveSliderNode>;

}