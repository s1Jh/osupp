#include "Game.hpp"

#include "Random.hpp"
#include "Standard.hpp"
#include "Util.hpp"

#include "AnimateTexture.hpp"

NS_BEGIN

int Game::start()
{
    log::custom("GREETING", "Hello, world!");

    int init = initialize();
    if (init != 0) {
        log::error("Failed initializing: ", init);
        return init;
    }

    auto map = resources.load<MapInfo>("test.osu");

    Standard std(*this);
    std.setMap(map.get());

    //MapInfo map;
    //map.addSpinner(5, 1.0f, 1.0f, 15.0f);
    //std.setMap(&map);

    frect field = {{0.95f, 0.95f}, {0.0f, 0.0f}};
    std.setPlayField(field);

    while (true) {
        double delta = timing.getDelta();

        Keyboard::update();
        Mouse::update();

        if (keyboard[Key::Esc].releasing)
            break;

        std.update(delta);

        gfx.begin();
        std.draw(gfx);
        gfx.end();

        if (!gfx.runTasks(delta))
            close();

        timing.await();
    }

    log::custom("GREETING", "Goodbye, world!");
    return close();
}

int Game::close()
{
    LOG_ENTER();

    df2::write(gameSettings, "settings.sdf");

    gfx.destroy();
    std::exit(0);
}

int Game::initialize()
{
    LOG_ENTER();

    log::info("Initializing ", TOSTRING(GAME_TITLE), " ver.", VERSION_MAJOR, '.', VERSION_MINOR, '.', VERSION_PATCH);
    log::info("Build: ", BUILD_TYPE, " (", BUILD_DATE, ' ', BUILD_TIME, ')');
    log::info("Target: ", PLATFORM, ", ", ARCH);

    df2::addAlias("GAMEDIR", std::filesystem::current_path());

    gameSettings = df2::read("settings.sdf");
    gameDefinition = df2::read("notosu.gdf");

    std::string skin = gameSettings["user"]["skin"].str("default");
    for (const auto &path: gameDefinition["searchPaths"]) {
        std::filesystem::path p(path.second.str());
        auto skinDirectory = p / "skins";
        resources.addSearchPath(skinDirectory);
        auto songDirectory = p / "songs";
        resources.addSearchPath(songDirectory);
        auto profilesDirectory = p / "profiles";
        resources.addSearchPath(profilesDirectory);
    }

    if (!gfx.create(gameSettings["gfx"])) {
        return 1;
    }
    resources.loadPersistentAssets();

    activeSkin = resources.get<Skin>(skin);

    Keyboard::setViewport(gfx.getWindowHandle());
    Mouse::setViewport(gfx.getWindowHandle());

    return 0;
}

Resources &Game::getResourcePool()
{ return resources; }

SkinP Game::getActiveSkin()
{ return activeSkin; }

NS_END

#ifdef WIN32
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
#elif LINUX

int main()
#endif
{
    PROJECT_NAMESPACE::Game game;
    return game.start();
}
