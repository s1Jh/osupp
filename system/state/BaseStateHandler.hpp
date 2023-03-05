// Copyright (c) 2022-2023 sijh (s1Jh.199[at]gmail.com)
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

#pragma once

#include "define.hpp"
#include "BaseState.hpp"
#include "Context.hpp"
#include "Timing.hpp"

#include <memory>
#include <string>

namespace PROJECT_NAMESPACE::detail {

class BaseStateHandler
{
public:
    enum class StateReturn {
        RESTART, EXIT
    };

    using StatePtr = std::unique_ptr<detail::BaseState>;

    BaseStateHandler();

    StateReturn enter();

    void setState(GameState state);
    [[nodiscard]] bool isRunning() const;
    void exit();
    void restart();

protected:
    virtual void onContextCreate(std::shared_ptr<Context> ctx);

    virtual void onExit();
    virtual void onStateChange(StatePtr& oldState, StatePtr& newState);

    virtual void onFrameUpdate();
    virtual void onFrameDraw();
    virtual void onFrameExit();

    inline StatePtr& getCurrentStatePtr() { return currentStatePtr; }
    inline std::shared_ptr<Context> getContext() { return context; }

private:
    static std::string Stringify(const GameState &state);
    bool running;
    StateReturn ret;
    Timing timing;
    log::Logger logger;
    std::shared_ptr<Context> context;
    GameState currentState;
    GameState nextState;
    StatePtr currentStatePtr;
};

}