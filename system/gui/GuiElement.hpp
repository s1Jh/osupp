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
// Created by sijh on 04.02.23.
//
#pragma once

#include "define.hpp"

#include <memory>
#include <string>

#include "GuiPreDecls.hpp"
#include "Traits.hpp"
#include "LambdaRender.hpp"
#include "df2.hpp"
#include "AnimationController.hpp"
#include "GuiCollections.hpp"
#include "GuiPrototype.hpp"
#include "GuiIContainsElements.hpp"

namespace PROJECT_NAMESPACE {

namespace gui::detail
{

class IElement : public IContainsElements
{
public:
    virtual ~IElement() = default;

    bool doCreate(const df2 &df);

    bool doUpdate(Root&);

    bool doDraw(video::LambdaRender&);

    video::AnimationController& getAnimations();

protected:
    IElement() = default;

    virtual bool create(const df2 &df);
    virtual bool update(Root&);
    virtual bool draw(video::LambdaRender&);

    [[nodiscard]] const frect& getPhysicalArea() const;
    [[nodiscard]] const fsize& getDimensions() const;
    [[nodiscard]] frect getParentArea() const;
    bool onCreate(Element element) final;

    const Mat3f& getTransform();

private:
    void recalculateAABB();
    void setParent(IElement* parent);
    std::optional<fvec2d> parseExpr(const std::string& str);

    bool drawRects{false};

    video::AnimationController animation;
    Mat3f transform{};
    uint8_t aabbRefresh{0};
    frect area{};
    fsize dimensions{};
    IElement *parent{nullptr};
};

}

}
