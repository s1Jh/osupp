#pragma once

#include "BaseStateHandler.hpp"

namespace PROJECT_NAMESPACE {

class Game : public detail::BaseStateHandler
{
protected:
    StatePtr createInitialState();

    void onFrameDraw();
};

}