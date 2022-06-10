#pragma once

#include "define.hpp"
#include "HitObject.tpp"
#include "SliderTemplate.hpp"

NS_BEGIN

    class Slider : public HitObject<ObjectTemplateSlider> {
        friend class SliderUtils;
    public:
        explicit Slider(std::shared_ptr<ObjectTemplateSlider>, BaseGameMode *);

        [[nodiscard]] fvec2d getStartPosition() const;
        [[nodiscard]] fvec2d getEndPosition() const;

    protected:
        [[nodiscard]] fvec2d findDirection(double t);
        [[nodiscard]] fvec2d findNormal(double t);

        void onReset() override;

        enum class TravelDirection {
            Forward, Backward
        };

        void onUpdate(double delta) override;
        void onBegin() override;
        HitResult onFinish() override;
        void onRaise() override;
        void onDraw(Renderer &renderer) override;
        void onPress() override;

    private:
        std::list<fvec2d> leftSide;
        std::list<fvec2d> rightSide;
        std::list<fvec2d> circlePoints;
        Mesh body;
        unsigned int stepsPerCurveUnit;
        // the interpolated path
        SliderPathT interpolatedPath;
        Curve<SliderPathT::iterator> curve;
        double epsilon;

        // cached variables
        float endBumperAngle;
        float startBumperAngle;

        // state
        TravelDirection currentDirection;
        bool started;
        unsigned int repeatsLeft;
        double progression;
        double curvePosition;
        double velocity;
    };

}
