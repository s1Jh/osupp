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
// Created by sijh on 13.02.23.
//

#include "GuiIContainsElements.hpp"

#include "GuiElement.hpp"

#include "Log.hpp"

namespace PROJECT_NAMESPACE {

namespace gui::detail
{

int IContainsElements::createElementsFrom(const df2 &def)
{
    elements.clear();

    int cnt = 0;
    for (auto &e : def) {
        auto &el = e.second;
        auto className = el["class"].str();

        if (className.empty()) {
            log::Error("Missing class name");
            continue;
        }

        auto it = std::find_if(
            Elements.begin(), Elements.end(), [&](const auto &proto) -> bool
            {
                if (proto->getId() == className) {
                    auto ptr = proto->createCopy();
                    add(ptr, el);
                    cnt++;
                    return true;
                }
                return false;
            }
        );

        if (it == Elements.end()) {
            log::Error("No match for class ", className);
        }
    }

    return cnt;
}
bool IContainsElements::updateElements(Root &root)
{
    bool ret = true;
    for (auto &elm : elements) {
        auto res = elm->doUpdate(root);
        ret = ret && res;
    }
    return ret;
}

bool IContainsElements::drawElements(video::LambdaRender &rnd)
{
    bool ret = true;
    for (auto &elm : elements) {
        auto res = elm->doDraw(rnd);
        ret = ret && res;
    }
    return ret;
}

IContainsElements::ElementStorage &IContainsElements::getElements()
{
    return elements;
}

bool IContainsElements::onCreate(IContainsElements::Element)
{
    return true;
}

std::weak_ptr<IElement> IContainsElements::add(const std::shared_ptr<IElement>& elm, const df2& def)
{
    if (onCreate(elm)) {
        if (elm->doCreate(def)) {
            elements.push_back(elm);
        }
    } else {
        log::Warning("Failed creating element ", elm);
    }
    return elm;
}

}

}
