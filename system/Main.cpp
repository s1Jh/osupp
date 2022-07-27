#include "define.hpp"

#include "Context.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

using namespace PROJECT_NAMESPACE;

#ifdef WIN32
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
#elif LINUX
int main()
#endif
{
    log::custom("GREETING", "Hello, world!");

    log::info("Initializing ", TOSTRING(GAME_TITLE), " ver.", VERSION_MAJOR, '.', VERSION_MINOR, '.', VERSION_PATCH);
    log::info("Build: ", BUILD_TYPE, " (", BUILD_DATE, ' ', BUILD_TIME, ')');
    log::info("Target: ", PLATFORM, ", ", ARCH);

    auto &ctx = GetContext();

    ctx.locale.loadFromFile("english.ldf");

    df2::addAlias("GAMEDIR", std::filesystem::current_path());

    ctx.settings = df2::read(CONFIG_PATH);

    if (!ctx.gfx.create()) {
        return 1;
    }
    ctx.resources.loadPersistentAssets();

    Keyboard::setViewport(ctx.gfx.getWindowHandle());
    Mouse::setViewport(ctx.gfx.getWindowHandle());

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsClassic();

    ImGui_ImplGlfw_InitForOpenGL(ctx.gfx.getWindowHandle(), true);
    ImGui_ImplOpenGL3_Init(GL_VERSION_PREPROCESSOR);

    ctx.state.setState(GameState::INITIAL_STATE);

    while (ctx.state.isRunning()) {
        double delta = ctx.timing.getDelta();

        Keyboard::update();
        Mouse::update();

        if (ctx.keyboard[Key::Ctrl + Key::Q].releasing)
            ctx.state.setState(GameState::Exit);

        ctx.state.update(delta);

        ctx.gfx.begin();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ctx.state.draw();
        ImGui::Render();
        auto data = ImGui::GetDrawData();
        if (data)
            ImGui_ImplOpenGL3_RenderDrawData(data);
        ctx.gfx.end();

        if (!ctx.gfx.runTasks(delta))
            ctx.state.exit();

        ctx.state.process();

        ctx.timing.await();
    }

    log::custom("GREETING", "Goodbye, world!");
    df2::write(ctx.settings, CONFIG_PATH);
    ctx.gfx.destroy();
    std::exit(0);
}