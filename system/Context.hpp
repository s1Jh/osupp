#pragma once

#include "define.hpp"

#define USER_CONTEXT_INCLUDES
#include "config.hpp"

#include "StateHandler.hpp"
#include "Renderer.dpp"
#include "Resources.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Timing.hpp"
#include "Locale.hpp"
#include "Settings.hpp"

NS_BEGIN

struct Context
{
    StateHandler state;
    Resources resources;
    Renderer gfx;
    Keyboard keyboard;
    Mouse mouse;
    Timing timing;
    Settings settings;
    Locale locale;

#define CONTEXT_FIELD(Type, Name) Type Name;
    USER_CONTEXT_FIELDS
#undef CONTEXT_FIELD
};

Context &GetContext();

NS_END