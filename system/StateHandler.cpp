#include "StateHandler.hpp"

#include "Log.hpp"

#define USER_STATE_INCLUDES
#include "config.hpp"

NS_BEGIN

StateHandler::StateHandler()
    : currentState(GameState::None),
      nextState(GameState::None),
      currentStatePtr(nullptr)
{}

int StateHandler::update(double delta)
{
    if (currentStatePtr == nullptr)
        return -1;

    int ret = currentStatePtr->update(delta);

    if (ret >= (int) GameState::None) {
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
    if (nextState != GameState::None && nextState != currentState) {
        log::info("Setting state to ", (unsigned int) nextState);

        if (currentStatePtr != nullptr)
            currentStatePtr->exit();

        auto oldState = currentState;
        currentState = nextState;

        switch (currentState) {
#define USER_STATE(Name) case GameState::Name : currentStatePtr = std::make_unique<State<GameState::Name>>(); break;
            USER_STATES
#undef USER_STATE

            case GameState::Exit:
            case GameState::None:
                currentStatePtr = nullptr;
                break;
            default:
                log::error("Invalid state: ", (int) currentState);
                break;
        }

        if (currentStatePtr)
            currentStatePtr->init(oldState);
    }
    nextState = GameState::None;
}

bool StateHandler::isRunning() const
{
    return currentState != GameState::Exit;
}
void StateHandler::exit()
{
    setState(GameState::Exit);
}

NS_END