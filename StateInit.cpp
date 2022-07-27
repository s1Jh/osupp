#include "StateInit.hpp"
#include "Standard.hpp"
#include "StateMainMenu.hpp"


NS_BEGIN

int State<GameState::Init>::update(double delta)
{
    return (int) GameState::MainMenu;
}

int State<GameState::Init>::draw()
{
    return 0;
}

int State<GameState::Init>::exit()
{
    return 0;
}

int State<GameState::Init>::init(GameState state)
{
    log::info("Initiating game...");

    auto &ctx = GetContext();
    auto &resources = ctx.resources;

    const auto &userSettings = ctx.settings["user"];
    const auto &paths = ctx.settings["paths"];

    std::string skin = userSettings["skin"].str("default");

    resources.addSearchPath(std::filesystem::current_path());
    for (const auto &path: paths) {
        std::filesystem::path p(path.second.str());
        auto skinDirectory = p / "skins";
        resources.addSearchPath(skinDirectory);
        auto profilesDirectory = p / "profiles";
        resources.addSearchPath(profilesDirectory);
    }

    resources.getDirectory("songs", ctx.maps);
    ctx.activeSkin = resources.get<Skin>(skin);
    ctx.activeGameMode = std::make_unique<Standard>();

    return 0;
}

NS_END