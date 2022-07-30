#pragma once

#include "define.hpp"
#include "State.hpp"
#include "Context.hpp"

NS_BEGIN

DECLARE_STATE(GameState::MainMenu)
{
public:
    explicit State();
    int update(double delta) override;
    int draw() override;
    int exit() override;
    int init(GameState state) override;

private:
    void showDebugControl();
    void showMainMenuTab();
    void showSettingsTab();

    Context &ctx;
};

NS_END