#pragma once

#include "define.hpp"
#include "HitObject.tpp"
#include "SliderTemplate.h"

#include <forward_list>

namespace GAME_TITLE {

    class Slider : public HitObject<ObjectTemplateSlider> {
    public:
        explicit Slider(std::shared_ptr<ObjectTemplateSlider>, BaseGameMode *);

    protected:
        void onUpdate(double delta) override;
        void onBegin() override;
        HitResult onFinish() override;
        void onRaise() override;
        void onDraw(Renderer &renderer) override;
        void onPress() override;
    };

}
