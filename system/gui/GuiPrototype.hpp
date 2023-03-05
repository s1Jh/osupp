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
// Created by sijh on 10.02.23.
//
#pragma once

#include "define.hpp"

#include "GuiPreDecls.hpp"

#include "df2.hpp"
#include "Log.hpp"
#include "Util.hpp"

#include <string>
#include <type_traits>
#include <memory>

#include "nameof.hpp"

namespace PROJECT_NAMESPACE {

namespace gui::detail
{

struct IPrototype
{
    [[nodiscard]] virtual std::string getId() const;
    [[nodiscard]] virtual std::shared_ptr<IElement> createCopy() const;
};

template<typename T> requires std::is_base_of_v<IElement, T>
struct Prototype: public IPrototype
{
    [[nodiscard]] std::string getId() const override
    {
        return id;
    }

    [[nodiscard]] std::shared_ptr<IElement> createCopy() const override
    {
        return std::make_shared<T>();
    }

protected:
    std::string id{LowerCaseCopy(std::string(NAMEOF_SHORT_TYPE(T)))};
};

}

}
