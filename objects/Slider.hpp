#pragma once

#include "define.hpp"
#include "HitObject.tpp"
#include "SliderTemplate.hpp"
#include "SliderUtils.hpp"

namespace GAME_TITLE {

    class Slider : public HitObject<ObjectTemplateSlider> {
        friend class SliderUtils;
    public:
        explicit Slider(std::shared_ptr<ObjectTemplateSlider>, BaseGameMode *);

    protected:

        void onUpdate(double delta) override;
        void onBegin() override;
        HitResult onFinish() override;
        void onRaise() override;
        void onDraw(Renderer &renderer) override;
        void onPress() override;

        Mesh body;
        SliderUtils util;

        double length;
        ActiveSliderPathT path;

    private:
        // the interpolated path
        bool started;
        double targetProgression;
        double velocity;
        double progression;
    };

}
