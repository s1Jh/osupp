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

#include "GuiButton.hpp"

#include "GuiRoot.hpp"

#include "Geometry.hpp"

namespace PROJECT_NAMESPACE {

namespace gui
{

bool Button::create(const df2 &df)
{
    // By default, we shouldn't explicitly create a shape for the button, we instead rely on
    // the parent defaulting to a full unit rectangle, effectively making the whole element
    // parented to this clickable.

    auto setupFunction = [](Function& f, const df2& df) {

        f.name = df["func"].str();
        for (const auto& arg : df["args"]) {
            f.args.push_back(arg.second.str());
        }
    };

    setupFunction(left, df["left"]);
    setupFunction(middle, df["middle"]);
    setupFunction(right, df["right"]);

    return true;
}

bool Button::update(Root &root)
{
    auto &io = root.getIO();
    if (math::Intersects(getPhysicalArea(), io.mouse.position())) {
        // A unified way to handle calling functions - in the future we may want to check
        // return values and such, we will be able to do that easily in this lambda.
        auto DoInvocation = [&](const Function& f) {
            root.invokeFunction(f.name, f.args);
        };

        // This isn't exactly the best way of doing this, but we will probably only ever handle
        // three mouse buttons. Also, as of writing this, there is no way to iterate over all mouse
        // buttons, so rather than wasting time implementing that, we just copy-paste 3 if statements.
        if (io.mouse.left().releasing) { DoInvocation(left); }
        if (io.mouse.right().releasing) { DoInvocation(right); }
        if (io.mouse.middle().releasing) { DoInvocation(middle); }
    }
    return true;
}

//bool Button::draw(video::LambdaRender &render)
//{
//    auto area = getArea();
//    fcircle circ = {area.size.w, area.position};
//    render.draw(DrawRect{circ, video::VisualAppearance{.fillColor = RED}, MAT3_NO_TRANSFORM<float>});
//    return true;
//}

}

}
