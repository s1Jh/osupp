
#define GAME_TITLE NotOSU
#define PROJECT_NAMESPACE org::sijh::NotOSU

#define USER_RESOURCE_POOLS \
    RESOURCE_POOL(MapInfo, maps) \
    RESOURCE_POOL(Skin, skins)

#ifdef USER_RESOURCE_INCLUDES
#include "MapInfo.hpp"
#include "Skin.hpp"
#endif
