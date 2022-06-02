#include "StandardResources.hpp"

#include "define.hpp"


namespace GAME_TITLE {

    Texture StandardResources::NoteBase;
    Texture StandardResources::NoteOverlay;
    Texture StandardResources::ApproachCircle;
    Texture StandardResources::SliderHead;
    Texture StandardResources::SliderTail;
    Texture StandardResources::SliderHeadRepeat;
    Texture StandardResources::SliderTailRepeat;
    Texture StandardResources::SliderBody;
    Texture StandardResources::SliderHitPoint;
    Texture StandardResources::Spinner;
    Texture StandardResources::SpinnerCenter;
    Texture StandardResources::SpinnerMeter;

    void StandardResources::Populate(Resources* pool) {
        StandardResources::NoteBase = pool->textures.get(NOTE_BASE_SPRITE);
        StandardResources::NoteOverlay = pool->textures.get(NOTE_OVERLAY_SPRITE);
        StandardResources::ApproachCircle = pool->textures.get(APPROACH_CIRCLE_SPRITE);
        StandardResources::SliderHead = pool->textures.get(SLIDER_HEAD_SPRITE);
        StandardResources::SliderTail = pool->textures.get(SLIDER_TAIL_SPRITE);
        StandardResources::SliderHeadRepeat = pool->textures.get(SLIDER_HEAD_REPEAT_SPRITE);
        StandardResources::SliderTailRepeat = pool->textures.get(SLIDER_TAIL_REPEAT_SPRITE);
        StandardResources::SliderBody = pool->textures.get(SLIDER_BODY_SPRITE);
        StandardResources::SliderHitPoint = pool->textures.get(SLIDER_HITPOINT_SPRITE);
        StandardResources::Spinner = pool->textures.get(SPINNER_SPRITE);
        StandardResources::SpinnerCenter = pool->textures.get(SPINNER_CENTER_SPRITE);
        StandardResources::SpinnerMeter = pool->textures.get(SPINNER_METER_SPRITE);
    }

}