/*******************************************************************************
 * Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#include "StateHandler.hpp"

#include "Log.hpp"

#define USER_STATE_INCLUDES
#include "config.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "State.hpp"

NS_BEGIN

StateHandler::StateHandler()
    : currentState(GameState::NONE),
      nextState(GameState::INITIAL_STATE),
      currentStatePtr(nullptr)
{}

int StateHandler::update()
{
    if (currentStatePtr == nullptr)
        return -1;

    int ret = currentStatePtr->update(time.getDelta());

    if (ret >= (int) GameState::NONE) {
        nextState = (GameState) ret;
    }
    return ret;
}

int StateHandler::draw()
{
    if (currentStatePtr == nullptr)
        return -1;
    else
        return currentStatePtr->draw();
}

void StateHandler::setState(GameState state)
{
    nextState = state;
}

void StateHandler::process()
{
    if (nextState != GameState::NONE && nextState != currentState) {
        log::info("Setting state to ", Stringify(nextState));

        std::unique_ptr<Context> contextPtr;

        if (currentStatePtr != nullptr) {
            currentStatePtr->exit();
            std::swap(contextPtr, currentStatePtr->ctx);
        } else {
            contextPtr = std::make_unique<Context>();
        }

        auto oldState = currentState;
        currentState = nextState;

        switch (currentState) {
#define USER_STATE(Name) case GameState::Name : currentStatePtr = std::make_unique<State<GameState::Name>>(); break;
            USER_STATES
#undef USER_STATE

            case GameState::EXIT:
            case GameState::NONE:
                currentStatePtr = nullptr;
                break;
            default:
                log::error("Invalid state: ", (int) currentState);
                break;
        }

        if (currentStatePtr != nullptr) {
            std::swap(contextPtr, currentStatePtr->ctx);
            currentStatePtr->init(oldState);
        }
    }
    nextState = GameState::NONE;
}

bool StateHandler::isRunning() const
{
    return currentState != GameState::EXIT;
}
void StateHandler::exit()
{
    setState(GameState::EXIT);
}

std::string StateHandler::Stringify(const GameState &state)
{
    switch (state) {
#define USER_STATE(State, ...) case GameState::State: return TOSTRING(State);
        ALL_STATES
#undef USER_STATE
	default:	return "Null";
    }
}

int StateHandler::operator()()
{
    process();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    update();

    draw();

    ImGui::Render();
    auto data = ImGui::GetDrawData();
    if (data)
        ImGui_ImplOpenGL3_RenderDrawData(data);

    time.await();
    return isRunning();
}

NS_END