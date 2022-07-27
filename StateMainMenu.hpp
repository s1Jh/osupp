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

    void iterateSettingsSearch(
        std::unordered_map<std::string, std::string> &entries, const df2 &clump, const std::string &previousName);

    Context &ctx;
};

NS_END