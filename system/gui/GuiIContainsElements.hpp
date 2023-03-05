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
#pragma once

#include "define.hpp"

#include "GuiPreDecls.hpp"
#include "GuiCollections.hpp"

#include "LambdaRender.hpp"
#include "df2.hpp"

#include <memory>
#include <type_traits>

namespace PROJECT_NAMESPACE {

namespace gui::detail
{

class IContainsElements : public Collections
{
public:
    using Element = std::shared_ptr<detail::IElement>;
    using ElementStorage = std::vector<Element>;

    template <typename T> requires std::is_base_of_v<detail::IElement, T>
    std::weak_ptr<T> add(const df2& def = df2::null) {
        auto wptr = add(std::make_shared<T>(), def);
        if (auto ptr = wptr.lock()) {
            return std::dynamic_pointer_cast<T>(ptr);
        }
    }

protected:
    int createElementsFrom(const df2& def);
    bool updateElements(Root& root);
    bool drawElements(video::LambdaRender& rnd);
    ElementStorage  &getElements();

    virtual bool onCreate(Element);

private:
    std::weak_ptr<IElement> add(const std::shared_ptr<IElement>& elm, const df2& def);

    ElementStorage elements;
};

}

}
