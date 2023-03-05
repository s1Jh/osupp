// Copyright (c) 2022-2023 sijh (s1Jh.199[at]gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "BaseStateHandler.hpp"

#include "Log.hpp"
#include "BaseState.hpp"
#include "Context.hpp"

#include "nameof.hpp"

#define USER_STATE_INCLUDES
#include "config.hpp"
#include "Error.hpp"

#include <memory>

namespace PROJECT_NAMESPACE::detail
{

BaseStateHandler::BaseStateHandler()
    : running(true),
      logger("STATE", log::Severity::INF),
      currentState(GameState::NONE),
      nextState(GameState::INITIAL_STATE),
      currentStatePtr(nullptr)
{
}

BaseStateHandler::StateReturn BaseStateHandler::enter()
{
    logger("Creating context");
    context = std::make_shared<Context>();

    if (!context->settings.read(CONFIG_PATH)) {
        logger(log::Severity::ERR, "Couldn't read the settings!");
    }

    if (context->gfx.init()) {
        if (!context->gfx.initImGui()) {
            log::Error("Couldn't initiate ImGui");
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

        auto
            msLevels = context->settings.addSetting<int>("setting.gfx.ms_levels", 0, SettingFlags::WRITE_TO_FILE, 0, 4);

        auto resolution = context->settings.addSetting<std::string>(
            "setting.gfx.window.resolution", "1920x1080", SettingFlags::WRITE_TO_FILE,
            StandardResolutions
        );
        auto
            fullscreen =
            context->settings.addSetting<bool>("setting.gfx.window.fullscreen", false, SettingFlags::WRITE_TO_FILE);
        auto
            monitor = context->settings.addSetting<int>("setting.gfx.window.monitor", 1, SettingFlags::WRITE_TO_FILE);
        auto
            refreshRate =
            context->settings
                   .addSetting<int>("setting.gfx.window.refresh_rate", 60, SettingFlags::WRITE_TO_FILE, 0, 120);
        context->settings.subscribeCallback<SettingCallbacks::SETTING_CHANGED>(
            wrap(
                [this, resolution, fullscreen, refreshRate, monitor](const std::string &setting)
                {
                    if (!setting.starts_with("setting.gfx.window")) {
                        return CallbackReturn::OK;
                    }

                    auto dimensions = GetCharacterSeparatedValues(resolution.get(), 'x');
                    auto width = GetParam(dimensions, 0, video::DEFAULT_WINDOW_SIZE.w);
                    auto height = GetParam(dimensions, 1, video::DEFAULT_WINDOW_SIZE.h);

                    video::WindowConfiguration config;
                    config.size = {width, height};
                    config.refreshRate = refreshRate.get();
                    config.monitorID = monitor.get();
                    config.mode = fullscreen.get() ? video::WindowMode::FULLSCREEN : video::WindowMode::WINDOWED;
                    config.shown = video::WindowVisibility::VISIBLE;

                    if (!context->gfx.configure(config)) {
                        log::Error("Failed to configure window!");
                    }

                    return CallbackReturn::OK;
                }
            ));
    } else {
        error::Raise(error::Code::API_FAIL_FATAL);
    }
    logger("Initiated video");

    auto locales = context->paths.findAll("locale", Resource<util::Locale>::allowedExtensions);
    std::vector<std::string> localeNames;
    for (const auto &locale : locales) {
        auto name = locale.stem();
        if (std::find(localeNames.begin(), localeNames.end(), name) == localeNames.end()) {
            localeNames.push_back(name.string());
        }
    }
    auto locale = context->settings.addSetting<std::string>(
        "setting.user.locale",
        std::string("english"),
        SettingFlags::WRITE_TO_FILE,
        localeNames
    );

    context->settings.subscribeCallback<SettingCallbacks::SETTING_CHANGED>(
        wrap(
            [this, locale](const std::string &setting)
            {
                if (setting == "setting.user.locale") {
                    auto locPath = context->paths.find(locale.get(), Resource<util::Locale>::allowedExtensions);
                    auto loc = Load<util::Locale>(locPath);
                    SetDefaultLocale(loc);
                    logger("Set locale ", loc->getLocName());
                }
                return CallbackReturn::OK;
            }
        ));

    logger("Initiated locale");

    auto devices = GetAudioDevices();
    std::vector<std::string> deviceNames;
    for (const auto &dev : devices) {
        deviceNames.push_back(dev.name);
    }
    auto audioDev =
        context->settings.addSetting<std::string>("setting.audio.device", "", SettingFlags::WRITE_TO_FILE, deviceNames);

    context->settings.subscribeCallback<SettingCallbacks::SETTING_CHANGED>(
        wrap(
            [this, audioDev](const std::string &setting)
            {
                if (setting == "setting.audio.device") {
                    context->audio = GetAudioDevice(audioDev.get(), context->settings);
                }
                return CallbackReturn::OK;
            }
        ));

    logger("Configured audio");

    auto frameRate = context->settings.addSetting<int>("state.fps", 60, SettingFlags::WRITE_TO_FILE, 10, 60);
    timing.setFramerate(frameRate.get());

    onContextCreate(context);
    context->settings.apply();
    logger("Finished context creation");

    while (isRunning()) {
        if (nextState != currentState) {
            logger("Setting state to", Stringify(nextState));

            if (currentStatePtr != nullptr) {
                currentStatePtr->exit();
            }

            auto oldState = currentState;
            currentState = nextState;

            StatePtr oldStatePtr;
            currentStatePtr.swap(oldStatePtr);

            switch (currentState) {
#define USER_STATE(Name) case GameState::Name : currentStatePtr = std::make_unique<State<GameState::Name>>(); break;
                USER_STATES
#undef USER_STATE
                // fall through
                case GameState::NONE:
                default:logger(log::Severity::ERR, "Invalid state: ", (int) currentState);
                    break;
            }

            currentStatePtr->ctx = context;
            currentStatePtr->sh = this;

            if (currentStatePtr != nullptr) {
                currentStatePtr->init(oldState);
            }

            onStateChange(oldStatePtr, currentStatePtr);
        }

        if (currentStatePtr) {
            onFrameUpdate();
            currentStatePtr->update(timing.getDelta());
            onFrameDraw();
            context->gfx.update();
            context->gfx.begin();
            currentStatePtr->draw();
            context->gfx.finish();
            onFrameExit();
        }

        timing.await();
    }

    if (!context->settings.write(CONFIG_PATH)) {
        logger(log::Severity::ERR, "Couldn't read the settings!");
    }

    return ret;
}

void BaseStateHandler::setState(GameState state)
{
    nextState = state;
}

bool BaseStateHandler::isRunning() const
{
    return running;
}

void BaseStateHandler::exit()
{
    running = false;
    ret = StateReturn::EXIT;
}

void BaseStateHandler::restart()
{
    running = false;
    ret = StateReturn::RESTART;
}

std::string BaseStateHandler::Stringify(const GameState &state)
{
#ifdef RELEASE
    return std::string(NAMEOF_ENUM(state));
#else
    switch (state) {
#define USER_STATE(State, ...) case GameState::State: return TOSTRING(State);
        USER_STATES
#undef USER_STATE
        default: return "Null";
    }
#endif
}

void BaseStateHandler::onContextCreate(std::shared_ptr<Context> ctx)
{}
void BaseStateHandler::onExit()
{}
void BaseStateHandler::onStateChange(BaseStateHandler::StatePtr &, BaseStateHandler::StatePtr &)
{}
void BaseStateHandler::onFrameUpdate()
{}
void BaseStateHandler::onFrameDraw()
{}
void BaseStateHandler::onFrameExit()
{}

}