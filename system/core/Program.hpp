
#pragma once

#include "define.hpp"

#include "EnumOperators.hpp"

#include <limits>

NS_BEGIN

namespace program {

enum class InitLayers : uint32_t {
    ERROR = 1 << 0,
    TASKS = 1 << 1,
    LOCALE = 1 << 2,
    AUDIO = 1 << 3,
    VIDEO = 1 << 4,
    IMGUI = 1 << 5
};

constexpr InitLayers INIT_ALL_LAYERS = static_cast<InitLayers>(std::numeric_limits<uint32_t>::max());

InitLayers Init(InitLayers layers = INIT_ALL_LAYERS);
void Exit(int exitCode = 0);
InitLayers GetInitializedLayers();

}

ENABLE_BITMASK_OPERATORS(program::InitLayers)

NS_END
