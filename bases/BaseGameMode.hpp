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

#include "BaseHitObject.hpp"
#include "Keyboard.hpp"
#include "MapInfo.hpp"
#include "Mouse.hpp"
#include "Renderer.dpp"
#include "Skin.hpp"

#include <list>

NS_BEGIN

class BaseGameMode
{
public:
    using StorageT = std::list<std::shared_ptr<BaseHitObject>>;

    explicit BaseGameMode();

    virtual void update(double delta);

    virtual void draw(Renderer &);

    [[nodiscard]] double getCurrentTime() const;

    void setCurrentTime(double newTime);

    [[nodiscard]] const frect &getPlayField() const;

    void setPlayField(const frect &playField);

    [[nodiscard]] MapInfoP getMap() const;

    void setMap(MapInfoP map);

    void reset();

    [[nodiscard]] fvec2d getCursorPosition() const;

    [[nodiscard]] const Mat3f &getObjectTransform() const;

    [[nodiscard]] float getCircleSize();

    [[nodiscard]] float getApproachTime();

    [[nodiscard]] float getFadeTime();

    [[nodiscard]] float getHitWindow();

protected:
    virtual void onUpdate(double delta) = 0;

    virtual void onDraw(Renderer &) = 0;

    virtual void onReset();

    StorageT::iterator last;
    StorageT activeObjects;
    MapInfoP info;
    Keyboard keyboard;
    Mouse mouse;

private:
    Mat3f transform;
    frect playField;
    double currentTime;
};

NS_END