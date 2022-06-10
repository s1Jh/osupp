#pragma once

#include "define.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include "Renderer.hpp"
#include "Timing.hpp"
#include "Resources.hpp"
#include "Standard.hpp"

NS_BEGIN

class Game {
public:
    Game();

    int start();

    int initialize();

    int close();

private:
    df2 gameSettings;
    df2 gameDefinition;
    Resources resources;
    Renderer gfx;
    Keyboard keyboard;
    Mouse mouse;
    Timing timing;
};

NS_END