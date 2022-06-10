#pragma once

#include "define.hpp"

#include "MapInfo.hpp"
#include <string>

NS_BEGIN

bool LoadMAP(const std::string& fpath, MapInfo& map);
bool LoadOSU(std::filesystem::path pathIn, MapInfo& map);

NS_END