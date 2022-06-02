#pragma once

#include "define.hpp"

#include "MapInfo.hpp"
#include <string>

namespace GAME_TITLE {

    bool LoadMAP(const std::string& fpath, MapInfo& map);

}