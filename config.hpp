
/*******************************************************************************
 * Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#define GAME_TITLE osu++
#define PROJECT_NAMESPACE osupp

#define CONFIG_FILE settings.sdf

#define USER_CONTEXT_FIELDS \
    CONTEXT_FIELD(MapManager, maps) \
    CONTEXT_FIELD(Resource<Skin>, activeSkin) \
    CONTEXT_FIELD(GameManager, game)

#ifdef USER_CONTEXT_INCLUDES
#include "Skin.hpp"
#include "Resource.hpp"
#include "GameManager.hpp"
#include "MapManager.hpp"
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

#ifdef USER_DRAW_INCLUDES
#include "ObjectSprite.hpp"
#endif

#define INITIAL_STATE Init
