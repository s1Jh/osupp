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
// Created by sijh on 15.02.23.
//

#include "BopToBpm.hpp"

#include "Time.hpp"

#include "Math.hpp"

namespace PROJECT_NAMESPACE {

namespace video
{


void BopToBPM::update(float, Transform2D &transform)
{
    auto t = time::SinceStart() - startTime;
    auto scale = math::Sin(t * bpm * time::SECONDS_MINUTES_RATIO * 2.0_pi);

    transform.scale = fvec2d{1.0, 1.0} * math::Lerp(1.1f, 0.9f, math::SmoothStep(scale));
}

bool BopToBPM::done()
{
    return false;
}

BopToBPM::BopToBPM(float bpmIn) : startTime((float)time::SinceStart()), bpm(bpmIn)
{}

}

}
