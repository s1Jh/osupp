#pragma once

#include "define.hpp"

#include <filesystem>
#include <vector>

namespace PROJECT_NAMESPACE {

namespace compat
{

struct OsuInstallation
{
    std::filesystem::path path;
    size_t skinCount;
    size_t songCount;
};

bool IsValidOsuInstallation(const std::filesystem::path& path);
std::vector<OsuInstallation> FindOsuInstallations();
bool ImportOsuData(const std::filesystem::path& osuDirectory);
bool ConvertSkin(const std::filesystem::path& sourcePath);

}

}
