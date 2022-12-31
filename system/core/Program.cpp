#include "Program.hpp"

#include "Log.hpp"
#include "Locale.hpp"
#include "Context.hpp"
#include "Error.hpp"
#include "Tasks.hpp"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include <atomic>

NS_BEGIN

namespace program
{

static std::atomic<InitLayers> InitiatedLayers;

bool InitImGui()
{
// #ifdef USE_IMGUI

    if (!GetContext().gfx.initImGui()) {
        log::error("Couldn't initiate ImGui");
        return false;
    }

// #endif // USE_IMGUI
    return true;
}

bool InitVideo()
{
    video::detail::InitPlatformVideo();

    Context &ctx = GetContext();
    if (!ctx.gfx.init()) {
        return false;
    }

    const std::vector<std::string> StandardResolutions = {
        "720x480",
        "960x540",
        "1280x720",
        "1366x768",
        "1600x900",
        "1920x1080",
        "2560x1440",
        "3200x1800",
        "3840x2160",
    };

    auto resolution = ctx.settings.addSetting<std::string>(
        "setting.gfx.window.resolution", "1920x1080", SettingFlags::WRITE_TO_FILE,
        StandardResolutions
    );
    auto
        fullscreen = ctx.settings.addSetting<bool>("setting.gfx.window.fullscreen", false, SettingFlags::WRITE_TO_FILE);
    auto
        monitor = ctx.settings.addSetting<int>("setting.gfx.window.monitor", 1, SettingFlags::WRITE_TO_FILE);
    auto
        refreshRate =
        ctx.settings.addSetting<int>("setting.gfx.window.refresh_rate", 60, SettingFlags::WRITE_TO_FILE, 0, 120);


    ctx.settings.subscribeCallback<SettingCallbacks::SETTING_CHANGED>(
        wrap(
            [resolution, fullscreen, refreshRate, monitor](const std::string &setting)
            {
                if (!setting.starts_with("setting.gfx.window")) {
                    return CallbackReturn::OK;
                }

                auto &ctx = GetContext();

                auto dimensions = GetCharacterSeparatedValues(resolution.get(), 'x');
                auto width = GetParam(dimensions, 0, video::DEFAULT_WINDOW_SIZE.w);
                auto height = GetParam(dimensions, 1, video::DEFAULT_WINDOW_SIZE.h);

                video::WindowConfiguration config;
                config.size = {width, height};
                config.refreshRate = refreshRate.get();
                config.monitorID = monitor.get();
                config.mode = fullscreen.get() ? video::WindowMode::FULLSCREEN : video::WindowMode::WINDOWED;
                config.shown = video::WindowVisibility::VISIBLE;

                if (!ctx.gfx.configure(config)) {
                    log::error("Failed to configure window!");
                }

                return CallbackReturn::OK;
            }
        ));


    log::info("Started video");
    return true;
}

bool InitAudio()
{
    Context &ctx = GetContext();

    auto devices = GetAudioDevices();
    std::vector<std::string> deviceNames;
    for (const auto &dev : devices) {
        deviceNames.push_back(dev.name);
    }
    auto audioDev =
        ctx.settings.addSetting<std::string>("setting.audio.device", "", SettingFlags::WRITE_TO_FILE, deviceNames);

    ctx.settings.subscribeCallback<SettingCallbacks::SETTING_CHANGED>(
        wrap(
            [audioDev](const std::string &setting)
            {
                Context &ctx = GetContext();
                if (setting == "setting.audio.device") {
                    ctx.audio = GetAudioDevice(audioDev.get());
                }
                return CallbackReturn::OK;
            }
        ));

    log::info("Configured audio");
    return true;
}

bool InitErrors()
{
    error::detail::InstallHandler();
    log::info("Installed error handler");
    return true;
}

bool InitTasks()
{
    tasks::Start();
    log::info("Started task system");
    return true;
}

bool InitLocale()
{
    auto &ctx = GetContext();

    auto locales = ctx.paths.findAll("locale", Resource<util::Locale>::allowedExtensions);
    std::vector<std::string> localeNames;
    for (const auto &locale : locales) {
        auto name = locale.stem();
        if (std::find(localeNames.begin(), localeNames.end(), name) == localeNames.end()) {
            localeNames.push_back(name.string());
        }
    }
    auto locale = ctx.settings.addSetting<std::string>(
        "setting.user.locale",
        std::string("english"),
        SettingFlags::WRITE_TO_FILE,
        localeNames
    );

    ctx.settings.subscribeCallback<SettingCallbacks::SETTING_CHANGED>(
        wrap(
            [locale](const std::string &setting)
            {
                Context &ctx = GetContext();
                if (setting == "setting.user.locale") {
                    auto locPath = ctx.paths.find(locale.get(), Resource<util::Locale>::allowedExtensions);
                    auto loc = Load<util::Locale>(locPath);
                    SetDefaultLocale(loc);
                    log::info("Set locale ", loc->getLocName());
                }
                return CallbackReturn::OK;
            }
        ));

    log::info("Initiated locale");

    return true;
}

InitLayers Init(InitLayers layers)
{
    log::custom("GREETING", "Hello, world!");

    log::detail::Init();

    log::info("Initializing ", TOSTRING(GAME_TITLE), " ver.", VERSION_MAJOR, '.', VERSION_MINOR, '.', VERSION_PATCH);
    log::info("Build: ", BUILD_TYPE, " (", BUILD_DATE, ' ', BUILD_TIME, ')');
    log::info("Target: ", PLATFORM, ", ", ARCH);

    Context &ctx = GetContext();

    auto currentPath = std::filesystem::current_path();
//    ctx.paths.addPath(currentPath); -- added automatically with default constructor
    df2::addAlias("GAMEDIR", currentPath.string());

    auto InitHelper = [&](InitLayers layer, const std::function<bool()> &f) -> bool
    {
        if (bool(layers & layer)) {
            if (f()) {
                InitLayers old = InitiatedLayers;
                old |= layer;
                InitiatedLayers = old;
                return true;
            }
        }
        return false;
    };

    InitHelper(InitLayers::ERROR, InitErrors);
    InitHelper(InitLayers::TASKS, InitTasks);
    InitHelper(InitLayers::LOCALE, InitLocale);
    InitHelper(InitLayers::AUDIO, InitAudio);

    if (InitHelper(InitLayers::VIDEO, InitVideo)) {
        // Only init imgui if video has been initated
        InitHelper(InitLayers::IMGUI, InitImGui);
    }

    ctx.settings.read();
    ctx.state.setState(GameState::INITIAL_STATE);

    return GetInitializedLayers();
}

InitLayers GetInitializedLayers()
{
    return InitiatedLayers;
}

void Exit(int exitCode)
{
    log::custom("GREETING", "Goodbye, world!");
    if (exitCode == 0) {
        Context &ctx = GetContext();
        ctx.settings.write();
    }

    tasks::Stop();
    std::exit(exitCode);
}

}

NS_END
