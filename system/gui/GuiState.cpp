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

#include "GuiState.hpp"

#include "GuiBuiltins.hpp"

namespace PROJECT_NAMESPACE {

namespace gui
{

bool State::update(Root &root)
{
    updateElements(root);
    updateModals(root);
    return true;
}

bool State::draw(video::LambdaRender &rnd)
{
    drawElements(rnd);
    drawModals(rnd);
    return true;
}

bool State::create(const df2 &def)
{
    phase = Phase::CLOSED;
    createElementsFrom(def["elements"]);
    createModalsFrom(def["modals"]);
    return true;
}

bool State::openModal(const std::string &name)
{
    return activateModal(name);
}

bool State::closeModal(const std::string &name)
{
    return deactivateModal(name);
}

void State::open(bool instant)
{
    if (instant) {
        phase = Phase::OPEN;
    }
}

void State::close(bool instant)
{
    if (instant) {
        phase = Phase::CLOSED;
    }
    for (auto &mod : getModals()) {
        mod.second.deactivate(instant);
    }

}

Phase State::getPhase() const
{
    return phase;
}

}

}
