#pragma once

#include "BaseState.hpp"

namespace PROJECT_NAMESPACE
{

class StateTest : public detail::BaseState
{
    int update(double delta) override;
    int draw() override;
    int exit() override;
    int init() override;
};

}