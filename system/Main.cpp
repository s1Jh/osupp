#include "define.hpp"

#include "Context.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <cstdio>

#include <AL/alut.h>
#include <AL/al.h>
#include <AL/alc.h>

using namespace PROJECT_NAMESPACE;

#ifdef WIN32
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
#elif LINUX
int main(int argc, char **argv)
#endif
{
	ALuint helloBuffer, helloSource;

	alutInitWithoutContext(&argc, argv);

    log::custom("GREETING", "Hello, world!");

    log::info("Initializing ", TOSTRING(GAME_TITLE), " ver.", VERSION_MAJOR, '.', VERSION_MINOR, '.', VERSION_PATCH);
    log::info("Build: ", BUILD_TYPE, " (", BUILD_DATE, ' ', BUILD_TIME, ')');
    log::info("Target: ", PLATFORM, ", ", ARCH);

    auto &ctx = GetContext();

    df2::addAlias("GAMEDIR", std::filesystem::current_path());
    ctx.resources.addSearchPath(std::filesystem::current_path());

    ctx.settings.read();

    auto locale = ctx.settings.addSetting<std::string>("setting.user.locale", std::string("english.ldf"));
    ctx.locale.loadFromFile(locale.get());

	auto devices =  GetAudioDevices();
	std::vector<std::string> deviceNames;
	for (const auto& dev : devices)
		deviceNames.push_back(dev.name);

	auto audioDev = ctx.settings.addSetting<std::string>("setting.audio.device", "", true, deviceNames);
	ctx.audio = GetAudioDevice(audioDev.get());

	helloBuffer = alutCreateBufferFromFile("test.wav");
	ALenum err = alutGetError();
	if (err != ALUT_ERROR_NO_ERROR) {
		log::error(alutGetErrorString(err));
	}
	alGenSources (1, &helloSource);
	alSourcei (helloSource, AL_BUFFER, helloBuffer);
	alSourcePlay (helloSource);

	ctx.settings.subscribeCallback<SettingCallbacks::SettingChanged>(wrap([&audioDev, &ctx](const std::string&) {
		ctx.audio = GetAudioDevice(audioDev.get());
		return CallbackReturn::Ok;
	}));

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

    ctx.settings.addSetting<bool>("test.bool1", false, false);
    ctx.settings.addSetting<bool>("test.bool2", true, false);
    ctx.settings.addSetting<bool>("test.bool3", false, false);
    ctx.settings.addSetting<bool>("test.bool4", true, false);
    ctx.settings.addSetting<bool>("test.bool5", false, false);
    ctx.settings.addSetting<bool>("test.bool6", false, false);

    ctx.settings.addSetting<color>("test.color1", PURPLE, false);
    ctx.settings.addSetting<color>("test.color2", GREEN, false);
    ctx.settings.addSetting<color>("test.color3", GRAY, false);
    ctx.settings.addSetting<color>("test.color4", BLUE, false);
    ctx.settings.addSetting<color>("test.color5", MAGENTA, false);
    ctx.settings.addSetting<color>("test.color6", TURQUOISE, false);

    ctx.settings.addSetting<color>("test.pallette", PURPLE, false,
                                   std::vector<color>{PURPLE, AZURE, BEIGE, COBALT_BLUE, BRONZE, GOLD, SILVER});

    auto backFill = ctx.settings.addSetting<color>("debug.backfill", BLACK, false);

    ctx.state.setState(GameState::INITIAL_STATE);

    while (ctx.state.isRunning()) {
        double delta = ctx.timing.getDelta();

        Keyboard::update();
        Mouse::update();

        if (ctx.keyboard[Key::Ctrl + Key::Q].releasing)
            ctx.state.setState(GameState::Exit);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ctx.state.update(delta);

        ctx.gfx.begin(backFill.get());
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
    ctx.settings.write();
    ctx.gfx.destroy();
    std::exit(0);
}