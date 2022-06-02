#pragma once

#include "define.hpp"

#include "Mesh.hpp"

#include <string>

namespace GAME_TITLE {

    bool LoadOBJ(const std::string &fpath, Mesh& mesh);

}