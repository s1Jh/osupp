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

/**
 * Gui collections.
 *
 * A single class that automatically stores an instance of a prototype for every GUI object.
 *
 * This solution is turning out to be quite clunky, perhaps in the future we will simply
 * create an interface for user to add their own classes and add builtin ones manually
 * through some kind of a global gui init function.
 */
#pragma once

#include "define.hpp"

#include "GuiPreDecls.hpp"
#include "GuiPrototype.hpp"

#include <memory>
#include <vector>

namespace PROJECT_NAMESPACE {

namespace gui
{

/**
 * Stores prototypes for every GUI element. The prototypes are added during static initialization by
 * initializing a static member in every element. All classes that may need to iterate through
 * GUI element classes at runtime should inherit from this class to access it's Elements member.
 */
class Collections
{
public:
    using ElementStorage = std::unique_ptr<detail::IPrototype>;

    static bool AddElementPrototype(ElementStorage elm);
protected:
    static std::vector<ElementStorage> Elements;
};

}

}
