#include "Game.hpp"

#include "StateTest.hpp"
#include "Log.hpp"

#include "DebugMenus.hpp"

#include "imgui.h"

namespace PROJECT_NAMESPACE
{

Game::StatePtr Game::createInitialState()
{
    return std::make_unique<StateTest>();
}

void Game::onFrameDraw()
{
    static bool show = false;
    context->gfx.draw(ImGuiWindow{DrawDebugMenu, "DEBUG", &show, ImGuiWindowFlags_MenuBar});
}

}