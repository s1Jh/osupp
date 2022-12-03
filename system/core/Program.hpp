
#pragma once

#include "define.hpp"

#include "EnumOperators.hpp"

#include <limits>

NS_BEGIN

namespace program {

enum class InitLayers : uint32_t {
    ERROR,
    LOCALE,
    AUDIO,
    VIDEO,
    IMGUI
};

constexpr InitLayers INIT_ALL_LAYERS = static_cast<InitLayers>(std::numeric_limits<uint32_t>::max());

InitLayers Init(InitLayers layers = INIT_ALL_LAYERS);
void Exit(int exitCode = 0);

}

ENABLE_BITMASK_OPERATORS(program::InitLayers)

NS_END
