
#define GAME_TITLE NotOSU
#define PROJECT_NAMESPACE org::sijh::NotOSU

#define CONFIG_FILE settings.sdf

#define USER_RESOURCE_POOLS \
    RESOURCE_POOL(MapInfo, maps) \
    RESOURCE_POOL(Skin, skins)

#ifdef USER_RESOURCE_INCLUDES
#include "MapInfo.hpp"
#include "Skin.hpp"
#endif

#define USER_CONTEXT_FIELDS \
    CONTEXT_FIELD(std::vector<MapInfoP>, maps) \
    CONTEXT_FIELD(SkinP, activeSkin) \
    CONTEXT_FIELD(std::unique_ptr<BaseGameMode>, activeGameMode)

#ifdef USER_CONTEXT_INCLUDES
#include "Skin.hpp"
#include "MapInfo.hpp"
#include "df2.hpp"
#include "BaseGameMode.hpp"
#include <memory>
#include <vector>
#endif

#define USER_STATES \
    USER_STATE(MainMenu) \
    USER_STATE(InGame) \
    USER_STATE(Init)

#ifdef USER_STATE_INCLUDES
#include "StateInit.hpp"
#include "StateInGame.hpp"
#include "StateMainMenu.hpp"
#endif

#define INITIAL_STATE Init
