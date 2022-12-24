#include "Program.hpp"

#include "Log.hpp"
#include "Locale.hpp"
#include "Context.hpp"
#include "Error.hpp"
#include "Tasks.hpp"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

NS_BEGIN

namespace program
{

bool InitImGui()
{
	Context &ctx = GetContext();

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsClassic();
//
//	ImGui_ImplGlfw_InitForOpenGL(TO_GLFW(ctx.gfx.getWindowHandle()), true);
//	ImGui_ImplOpenGL3_Init(GL_VERSION_PREPROCESSOR);
	return true;
}

bool InitVideo()
{
	Context &ctx = GetContext();

	if (!ctx.gfx.create()) {
		return false;
	}

	ctx.keyboard.setViewport(ctx.gfx.getWindowHandle());
	ctx.mouse.setViewport(ctx.gfx.getWindowHandle());

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

	ctx.audio = GetAudioDevice(audioDev.get());
	log::info("Configured audio");

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

	return true;
}

bool InitErrors()
{
	error::detail::InstallHandler();
	return true;
}

bool InitTimers()
{
//    if (!StartTimerThread()) {
//        log::error("Failed to start timers");
//        return -1;
//    }
	return true;
}

bool InitTasks()
{
	tasks::Start();
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

	auto loc = Load<util::Locale>(locale.get());
	SetDefaultLocale(loc);
	log::info("Set locale ", loc->getLocName());

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

	auto initialized = static_cast<InitLayers>(0);

	auto InitHelper = [&](InitLayers layer, const std::function<bool()> &f) -> bool
	{
		if (bool(layers & layer)) {
			if (f()) {
				initialized |= layer;
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

	return initialized;
}

void Exit(int exitCode)
{
	log::custom("GREETING", "Goodbye, world!");
	if (exitCode == 0) {
		Context &ctx = GetContext();
		ctx.settings.write();
	}

	tasks::Stop();
//    StopTimerThread();
	std::exit(exitCode);
}

}

NS_END
