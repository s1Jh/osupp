#pragma once

#include "define.hpp"

#include "Texture.hpp"
#include "Shader.hpp"
#include "Resources.hpp"

namespace GAME_TITLE {

    struct StandardResources {
        static void Populate(Resources* pool);

        static Texture NoteBase;
        static Texture NoteOverlay;
        static Texture ApproachCircle;
        static Texture SliderHead;
        static Texture SliderTail;
        static Texture SliderHeadRepeat;
        static Texture SliderTailRepeat;
        static Texture SliderBody;
        static Texture SliderHitPoint;
        static Texture Spinner;
        static Texture SpinnerCenter;
        static Texture SpinnerMeter;
        static Texture SliderBall;
        static Texture PlayField;

        static Shader SliderShader;
    };
}
