#include "StateInGame.hpp"

#include "Random.hpp"
#include "Standard.hpp"
#include "Util.hpp"

#include "Context.hpp"

NS_BEGIN

int State<GameState::InGame>::update(double delta)
{
    ctx.activeGameMode->update(delta);

    if (ctx.keyboard[Key::Esc].releasing)
        ctx.state.setState(GameState::MainMenu);

    return 0;
}

int State<GameState::InGame>::draw()
{
    ctx.activeGameMode->draw(ctx.gfx);
    return 0;
}

int State<GameState::InGame>::exit()
{
    return 0;
}

int State<GameState::InGame>::init(GameState state)
{
    float ratio = 512.f / 384.f;

    float base = 0.8f;
    field = {{base * ratio, base}, {0.0f, 0.0f}};
    ctx.activeGameMode->setPlayField(field);
    return 0;
}

State<GameState::InGame>::State()
    : ctx(GetContext())
{}

NS_END

