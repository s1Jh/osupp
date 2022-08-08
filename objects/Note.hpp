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

#include "HitObject.tpp"
#include "ObjectSprite.hpp"
#include "NoteTemplate.hpp"
#include "define.hpp"

NS_BEGIN

class Note: public HitObject<ObjectTemplateNote>
{
public:
    explicit Note(std::shared_ptr<ObjectTemplateNote>, BaseGameMode &);

    [[nodiscard]] fvec2d getStartPosition() const override;

    [[nodiscard]] fvec2d getEndPosition() const override;

    [[nodiscard]] double getStartTime() const override;

    [[nodiscard]] double getEndTime() const override;

protected:
    void onReset() override;

    void onUpdate(double delta) override;

    void onDraw(Renderer &renderer) override;

    void onBegin() override;

    HitResult onFinish() override;

private:
    bool wasHit = false;
    ObjectSprite noteBase;
    ObjectSprite noteOverlay;
    ObjectSprite noteUnderlay;
};

NS_END