#include "Game.hpp"
#include "Util.hpp"
#include "Random.hpp"

NS_BEGIN

Game::Game() {

}

int Game::start() {
    int init = initialize();
    if (init != 0) {
        log::error("Failed initializing: ", init);
        return init;
    }

    Standard std;
    MapInfo info;

    //for (int i = 0; i < 5; i++)
    //    info.addNote(Random::Vec2<float>({-1, -1}, {1, 1}), false, i + 4);

    frect field = {
            {0.9f, 0.9f},
            {0.0f, 0.0f}
    };
    std.setPlayField(field);

    while (true) {
        double delta = timing.getDelta();

        Keyboard::update();
        Mouse::update();

        if (keyboard[Key::Esc].releasing)
            break;

        std.update(delta);

        if (keyboard[Key::K1].releasing) std.setCurrentTime(1.0);
        if (keyboard[Key::K2].releasing) std.setCurrentTime(2.0);
        if (keyboard[Key::K3].releasing) std.setCurrentTime(3.0);
        if (keyboard[Key::K4].releasing) std.setCurrentTime(4.0);
        if (keyboard[Key::K5].releasing) std.setCurrentTime(5.0);
        if (keyboard[Key::K6].releasing) std.setCurrentTime(6.0);
        if (keyboard[Key::K7].releasing) std.setCurrentTime(7.0);
        if (keyboard[Key::K8].releasing) std.setCurrentTime(8.0);
        if (keyboard[Key::K9].releasing) std.setCurrentTime(9.0);
        if (keyboard[Key::K0].releasing) std.setCurrentTime(10.0);

        if (keyboard[Key::Y].releasing) {
            info.clear();
            info.addSlider(
                    {
                            {Random::Vec2<float>({-1, -1}, {1, 1}),  true},
                            {Random::Vec2<float>({-1, -1}, {1, 1}),  true},
                            {Random::Vec2<float>({-1, -1}, {1, 1}),  true},
                            {Random::Vec2<float>({-1, -1}, {1, 1}),  true},
                            {Random::Vec2<float>({-1, -1}, {1, 1}),  true},
                            {Random::Vec2<float>({-1, -1}, {1, 1}),  true},
                            {Random::Vec2<float>({-1, -1}, {1, 1}),  true},
                            {Random::Vec2<float>({-1, -1}, {1, 1}),  true},
                    },
                    true, 0.0, 10, CurveType::Bezier, 3);
            std.setMap(&info);
            std.setCurrentTime(0.0);
        }

        gfx.begin();
        std.draw(gfx);
        gfx.end();

        if (!gfx.runTasks(delta))
            close();

        timing.await();
    }

    return close();
}

int Game::close() {
    gfx.destroy();
    std::exit(0);
}

int Game::initialize() {
    df2::addAlias("GAMEDIR", std::filesystem::current_path());

    gameSettings = df2::read("settings.sdf");
    gameDefinition = df2::read("notosu.gdf");

    for (const auto &path: gameDefinition["searchPaths"]) {
        log::info("Adding search path: ", path.second.str());
        resources.addSearchPath(path.second.str());
    }

    if (!gfx.create(gameSettings["gfx"])) {
        return 1;
    }
    resources.loadPersistentAssets();
    gfx.setResources(&resources);

    log::info("Loading gamemode resources");
    StandardResources::Populate(&resources);

    Keyboard::setViewport(gfx.getWindowHandle());
    Mouse::setViewport(gfx.getWindowHandle());

    return 0;
}

NS_END

#ifdef WIN32
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#elif LINUX
int main()
#endif
{
    GAME_TITLE::Game game;
    return game.start();
}
