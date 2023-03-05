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

#include "GuiCollections.hpp"
#include "GuiState.hpp"
#include "GuiModal.hpp"
#include "GuiIContainsElements.hpp"
#include "GuiIContainsModals.hpp"
#include "GuiRegistry.hpp"

#include "LambdaRender.hpp"
#include "df2.hpp"
#include "Log.hpp"

#include <vector>
#include <memory>
#include <unordered_map>
#include <mutex>

namespace PROJECT_NAMESPACE {

namespace gui
{

class Root : public detail::IContainsModals
{
public:
    Root();

    State& addState(const std::string &name, const df2& def = df2::null);
    bool addFunction(const std::string& name, sioFunc&& func);

    bool create(const df2& def = df2::null);

    bool update(double delta);
    bool draw(video::LambdaRender&);

    bool setState(const std::string& name);

    bool openModal(const std::string& name);
    bool closeModal(const std::string& name);

    bool invokeFunction(const std::string& func, const std::vector<std::string>& params);

    IO &getIO();
    double getDelta();
    [[nodiscard]] const log::Logger& getLog() const;

private:
    log::Logger guiLogger{"GUI", log::Severity::INF};
    double delta;
    IO io;
    Registry registry;
    State *activeState{nullptr};
    std::unordered_map<std::string, sioFunc> functions;
    std::unordered_map<std::string, State> states;
};

}

}