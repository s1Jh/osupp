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

#include "GuiRoot.hpp"

namespace PROJECT_NAMESPACE {

namespace gui
{

State &Root::addState(const std::string &name, const df2 &def)
{
    bool wasEmpty = states.empty();
    bool isNew = !states.contains(name);
    auto &state = states[name];
    if (isNew) {
        if (!def.isEmpty()) {
            if (!state.create(def)) {
                log::Error("An error occurred whilst creating state ", name);
            }
        }
    }

    if (wasEmpty) {
        setState(name);
    }

    return state;
}

bool Root::setState(const std::string &name)
{
    if (!states.contains(name)) {
        return false;
    }

    activeState = &states.at(name);
    return true;
}

bool Root::addFunction(const std::string &name, sioFunc &&func)
{
    bool contained = functions.contains(name);
    functions[name] = func;
    return !contained;
}

bool Root::update(double newDelta)
{
    delta = newDelta;
    bool mods = updateModals(*this);

    if (activeState) {
        io.keyboard.update();
        io.mouse.update();

        return mods && activeState->update(*this);
    }

    return false;
}

bool Root::draw(video::LambdaRender &rnd)
{
    bool mods = drawModals(rnd);
    if (activeState) {
        return mods && activeState->draw(rnd);
    }
    return false;
}

bool Root::openModal(const std::string &name)
{
    if (!activateModal(name)) {
        if (activeState) {
            return activeState->openModal(name);
        }
    } else {
        return true;
    }
    return false;
}

bool Root::closeModal(const std::string &name)
{
    if (!deactivateModal(name)) {
        if (activeState) {
            return activeState->closeModal(name);
        }
    } else {
        return true;
    }
    return false;
}

bool Root::invokeFunction(const std::string &func, const std::vector<std::string> &params)
{
    guiLogger("Invoking function", func);
    sioContext ctx;
    if (!functions.contains(func)) {
        return false;
    }

    auto &f = functions.at(func);
    return f(ctx, params);
}

bool Root::create(const df2 &def)
{
    states.clear();
    activeState = nullptr;

    createModalsFrom(def["modals"]);

    for (const auto &state : def["states"]) {
        addState(state.first, state.second);
    }
    return true;
}

IO &Root::getIO()
{
    return io;
}

Root::Root()
{
    addFunction(
        "ActivateModal", [&](sioContext &, const sioArguments &args) -> bool
        {
            return openModal(GetParam<std::string>(args, 0, ""));
        }
    );
    addFunction(
        "DeactivateModal", [&](sioContext &, const sioArguments &args) -> bool
        {
            return closeModal(GetParam<std::string>(args, 0, ""));
        }
    );
    addFunction(
        "SetState", [&](sioContext &, const sioArguments &args) -> bool
        {
            return setState(GetParam<std::string>(args, 0, ""));
        }
    );
}

double Root::getDelta()
{
    return delta;
}

const log::Logger &Root::getLog() const
{
    return guiLogger;
}

}

}
