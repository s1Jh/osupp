#pragma once

#include "define.hpp"

#include "Standard.hpp"
#include "State.hpp"

NS_BEGIN

class Context;

DECLARE_STATE(GameState::InGame)
{
public:
    explicit State();

    int update(double delta) override;
    int draw() override;
    int exit() override;
    int init(GameState state) override;

private:
    frect field;
    Context &ctx;
};

NS_END