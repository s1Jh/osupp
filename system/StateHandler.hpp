#pragma once

#include "define.hpp"
#include "State.hpp"

#include <memory>

NS_BEGIN

class StateHandler
{
public:
    StateHandler();

    int update(double delta);
    int draw();

    void process();
    void setState(GameState state);

    [[nodiscard]] bool isRunning() const;
    void exit();

private:

    static std::string Stringify(const GameState &state);
    GameState currentState;
    GameState nextState;
    std::unique_ptr<detail::BaseState> currentStatePtr;
};

NS_END