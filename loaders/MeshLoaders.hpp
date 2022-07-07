#pragma once

#include "define.hpp"

#include "Mesh.hpp"

#include <string>

NS_BEGIN

bool LoadOBJ(const std::string &fpath, Mesh &mesh);

NS_END