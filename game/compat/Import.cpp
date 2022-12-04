#include "Import.hpp"

#include "Context.hpp"

#include <array>

NS_BEGIN

namespace compat
{

bool IsValidOsuInstallation(const std::filesystem::path &path)
{
    if (!std::filesystem::is_directory(path)) {
        return false;
    }

    if (!std::filesystem::is_regular_file(path / "osu!.exe")) {
        return false;
    }

    if (!std::filesystem::is_directory(path / "Songs")) {
        return false;
    }

    if (!std::filesystem::is_directory(path / "Skins")) {
        return false;
    }

    return true;
}

std::vector<OsuInstallation> FindOsuInstallations()
{
    std::vector<OsuInstallation> ret;

    auto TryPath = [&](const std::filesystem::path &path)
    {
        if (!IsValidOsuInstallation(path)) {
            return;
        }

        ret.emplace_back();
        auto &install = ret.back();
        install.path = path;
        install.skinCount = install.songCount = 0;

        for (const auto &file : std::filesystem::directory_iterator(path / "Songs")) {
            if (file.is_directory()) {
                install.songCount++;
            }
        }
        for (const auto &file : std::filesystem::directory_iterator(path / "Skins")) {
            if (file.is_directory()) {
                install.skinCount++;
            }
        }
    };


#ifdef LINUX
    std::filesystem::path homePath(getenv("HOME"));
    TryPath(homePath / ".local" / "share" / "osu-stable");
    TryPath(homePath / ".local" / "share" / "osu");
    TryPath("/usr/share/osu-stable");
    TryPath("/usr/share/osu");
    TryPath("/opt/osu");
    TryPath("/opt/osu-stable");
#elif defined(WINDOWS)
    std::filesystem::path homePath(getenv("LOCALAPPDATA"));
    TryPath(homePath/"osu!");
#endif

    return ret;
}

bool ImportOsuData(const std::filesystem::path &path)
{
    if (!IsValidOsuInstallation(path)) {
        return false;
    }

    Context &ctx = GetContext();

    auto songPath = path / "Songs";
    auto localSongPath = std::filesystem::current_path() / "songs";

    bool allGood = true;

    std::error_code ec;
    std::filesystem::copy(
        songPath, localSongPath,
        std::filesystem::copy_options::skip_existing |
            std::filesystem::copy_options::recursive,
        ec
    );
    if (!ec) {
        allGood = false;
        log::error("Failed to copy song: ", ec.message());
    }

    auto skinPath = path / "Skins";

    for (const auto& skin : std::filesystem::directory_iterator(skinPath)) {
        if (!ConvertSkin(skin)) {
            allGood = false;
            log::error("Failed to import skin ", skin);
        }
    }
    return allGood;
}

bool ConvertSkin(const std::filesystem::path &sourcePath)
{

    return true;
}

}

NS_END
