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
#pragma once

#include "define.hpp"

#include "IAnimation.hpp"
#include "Video.hpp"

#include <memory>
#include <list>

namespace PROJECT_NAMESPACE {

namespace video
{

class AnimationController
{
public:
    using Animation = std::unique_ptr<detail::IAnimation>;
    using AnimationStorage = std::list<Animation>;

    template<typename T, typename ... Args>
    requires std::is_base_of_v<detail::IAnimation, T> && std::is_constructible_v<T, Args...>
    void addAnimation(Args ... args)
    {
        animations.push_back(std::make_unique<T>(args...));
    }

    bool popAnimation();
    void clearAnimations();

    template<typename T, typename ... Args>
    requires std::is_base_of_v<detail::IAnimation, T> && std::is_constructible_v<T, Args...>
    void setBaseAnimation(Args ... args)
    {
        baseAnimation = std::make_unique<T>(args...);
    }

    void clearBaseAnimation();

    void initialize(const frect& initialConditions);

    void update(float delta);

    Mat3f getMatrix() const;

private:
    Transform2D transform;
    AnimationStorage animations;
    Animation baseAnimation;
};

}

}
