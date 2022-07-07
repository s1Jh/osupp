#pragma once

#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Renderer.hpp"
#include "ResourcePile.hpp"
#include "Resources.hpp"
#include "Skin.hpp"
#include "Timing.hpp"
#include "define.hpp"

NS_BEGIN

class Game
{
public:
    Game();

    int start();

    int initialize();

    int close();

    Resources &getResourcePool();

    SkinP getActiveSkin();

private:
    SkinP activeSkin;
    df2 gameSettings;
    df2 gameDefinition;
    Resources resources;
    Renderer gfx;
    Keyboard keyboard;
    Mouse mouse;
    Timing timing;
};

NS_END