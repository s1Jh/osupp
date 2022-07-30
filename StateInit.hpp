#pragma once

#include "define.hpp"

#include "State.hpp"
#include "Context.hpp"

NS_BEGIN

DECLARE_STATE(GameState::Init)
{
public:
    int update(double delta) override;
    int draw() override;
    int exit() override;
    int init(GameState state) override;

private:
    int lastLoaded = 0;
    std::vector<std::filesystem::path> filesToLoad;
};

NS_END