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

#include "GuiElement.hpp"

#include "MoveLinear.hpp"
#include "Constraint.hpp"
#include "BopToBpm.hpp"
#include "CircleAbout.hpp"
#include "Random.hpp"
#include "LookAt.hpp"
#include "MatrixMath.hpp"

#include "GuiRoot.hpp"

#include <map>

namespace PROJECT_NAMESPACE {

namespace gui::detail
{

bool IElement::doUpdate(Root &root)
{
    animation.update(root.getDelta());

    if (parent) {
        transform =
            math::MakeScaleMatrix(fvec2d(dimensions))*
                animation.getMatrix()
            * parent->getTransform();
    } else {
        transform = math::MakeScaleMatrix((fvec2d)dimensions) * animation.getMatrix();
    }

    if (aabbRefresh <= 0) {
        recalculateAABB();
        aabbRefresh = 10;
    }
    aabbRefresh--;

    auto ret = updateElements(root);

    if (root.getIO().keyboard[input::Key::F3].releasing) {
        drawRects = !drawRects;
    }

    return this->update(root) && ret;
}

void IElement::recalculateAABB()
{
    fvec3d tl = {-1.f, +1.f, 1.f};
    fvec3d tr = {+1.f, +1.f, 1.f};
    fvec3d bl = {-1.f, -1.f, 1.f};
    fvec3d br = {+1.f, -1.f, 1.f};

    tl = fvec3d(transform * (Mat1x3f) tl);
    tr = fvec3d(transform * (Mat1x3f) tr);
    bl = fvec3d(transform * (Mat1x3f) bl);
    br = fvec3d(transform * (Mat1x3f) br);

    auto top = math::Max(tl[1], tr[1], bl[1], br[1]);
    auto bottom = math::Min(tl[1], tr[1], bl[1], br[1]);

    auto right = math::Max(tl[0], tr[0], bl[0], br[0]);
    auto left = math::Min(tl[0], tr[0], bl[0], br[0]);

    area = {
        {right - left, top - bottom},
        {(right + left) / 2.f, (top + bottom) / 2.f}
    };
}

bool IElement::doDraw(video::LambdaRender &rnd)
{
    auto ret = this->draw(rnd) && drawElements(rnd);

    if (drawRects) {
        color visual = PURPLE, physical = GREEN;
        visual.a = physical.a = 0.2f;
        auto rect = getPhysicalArea();
        auto parentRect = getParentArea();
//        rnd.draw(DrawRect{{dimensions}, video::VisualAppearance{.fillColor = visual}, transform});
        rnd.draw(DrawRect{rect, video::VisualAppearance{.fillColor = physical}, MAT3_NO_TRANSFORM<float>});
        rnd.draw(
            DrawLine{
                {rect.position, parentRect.position},
                video::VisualAppearance{},
                MAT3_NO_TRANSFORM<float>
            }
        );
    }

    return ret;
}

bool IElement::doCreate(const df2 &df)
{
    auto pos = parseExpr(df["pos"].str());
    auto size = parseExpr(df["size"].str());

    if (pos) {
        area.position = pos.value();
    }
    if (size) {
        dimensions = area.size = (fsize) size.value();
    } else {
        dimensions = area.size = {1, 1};
    }

    animation.initialize(area);

    createElementsFrom(df["elements"]);
    return create(df);
}

bool IElement::create(const df2 &)
{
    return false;
}

bool IElement::update(Root &)
{
    return false;
}

bool IElement::draw(video::LambdaRender &)
{
    return false;
}

void IElement::setParent(IElement *newParent)
{
    parent = newParent;
}

const frect &IElement::getPhysicalArea() const
{
    return area;
}

const fsize &IElement::getDimensions() const
{
    return dimensions;
}

frect IElement::getParentArea() const
{
    if (parent) {
        return parent->getPhysicalArea();
    } else {
        return UNIT_RECT<float>;
    }
}

bool IElement::onCreate(IContainsElements::Element element)
{
    element->setParent(this);
    return true;
}

const Mat3f &IElement::getTransform()
{
    return transform;
}

std::optional<fvec2d> IElement::parseExpr(const std::string &expressionIn)
{
    /**
     *
     * Vector forms:
     *  X:Y - default
     *
     *  X - apply one dimension to both values
     *
     * size arguments:
     *
     *  %X - percentile (0-100) relative to respective parent dimension
     *      %Xv - Vertical dimension
     *      %Xh - Horizontal dimension
     *
     *  X - absolute value (0-1000)
     *
     * algebra:
     *
     *  X+Y X-Y - Added or subtracted from lhs, X and Y are both vectors.
     *
     * anchor values:
     *
     *  l - Left middle            tl---t---tr
     *  r - Right middle            |       |
     *  t - Top middle              l   c   r
     *  b - Bottom middle           |       |
     *  c - Center                 bl---b---br
     *
     *  tl - Top left
     *  tr - Top right
     *  br - Bottom right
     *  bl - Bottom left
     *
     */

    const static std::map<std::string, fvec2d> ANCHORS = {
        {"TL", fvec2d{-1, 1}},   // top left
        {"T", fvec2d{0, 1}},     // top middle
        {"TR", fvec2d{1, 1}},    // top right
        {"L", fvec2d{-1, 0}},    // left
        {"C", fvec2d{0, 0}},     // center
        {"R", fvec2d{1, 0}},     // right
        {"BL", fvec2d{-1, -1}},  // bottom left
        {"B", fvec2d{0, -1}},    // bottom middle
        {"BR", fvec2d{1, -1}}    // bottom right
    };

    auto ParseComponent = [&](std::string &comp) -> std::optional<float>
    {
        // Modify the string for easier comparisons
        Trim(comp);

        bool rel = false;
        // Relative value?
        if (comp.starts_with('%')) {
            // get rid of the leading %
            comp.erase(comp.begin());
            rel = true;
        }

        char *end;
        int val = std::strtol(comp.data(), &end, 10);

        if (comp.data() == end) {
            // error, couldn't read any numbers in
            return {};
        }

        if (!rel) {
            // Absolute position
            return ((float) val / 1000.f);
        } else {
            // Relative position
            return float(val) / 100.f;
        }
    };

    auto ParseVector = [&](const std::string_view &vec) -> std::optional<fvec2d>
    {
        auto comps = GetCharacterSeparatedValues(std::string(vec), ':');

        if (comps.size() == 1) {
            // Apply
            auto r = ParseComponent(comps[0]);
            if (r) {
                return {{r.value(), r.value()}};
            } else {
                return {};
            }
        } else if (comps.size() == 2) {
            auto x = ParseComponent(comps[0]);
            auto y = ParseComponent(comps[1]);

            if (x && y) {
                return {{x.value(), y.value()}};
            } else {
                return {};
            }
        }

        return {};
    };

    auto IsOp = [](char x) -> bool
    { return (x == '+') || (x == '-'); };

    auto ResolveVector = [&](const std::string_view &view) -> std::optional<fvec2d>
    {

        std::string str(view);
        Trim(str);

        for (const auto &anchor : ANCHORS) {
            if (anchor.first == str) {
                return {anchor.second};
            }
        }

        return ParseVector(view);
    };

    auto expression = UpperCaseCopy(expressionIn);
    fvec2d sum;

    std::string::iterator head = std::find_if(expression.begin(), expression.end(), IsOp);
    std::string_view first(expression.begin(), head);

    auto v = ResolveVector(first);
    if (v) {
        sum = v.value();
    } else {
        return {};
    }

    if (head == expression.end()) {
        return sum;
    }

    while ((head = std::find_if(head, expression.end(), IsOp)) != expression.end()) {
        auto op = *head;

        head++;

        std::string_view after(head, std::find_if(head, expression.end(), IsOp));

        auto addition = ResolveVector(after);
        if (addition) {
            auto &v = addition.value();
            switch (op) {
                case '+': sum += v;
                    break;
                case '-': sum -= v;
                    break;
                default: return {};
            }
        } else {
            return {};
        }
    }

    return sum;
}

video::AnimationController &IElement::getAnimations()
{
    return animation;
}

}

}