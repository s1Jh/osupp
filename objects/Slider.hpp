#pragma once

#include "HitObject.tpp"
#include "SliderTemplate.hpp"
#include "define.hpp"

NS_BEGIN

class Slider: public HitObject<ObjectTemplateSlider>
{
    friend class SliderUtils;

public:
    explicit Slider(std::shared_ptr<ObjectTemplateSlider>, BaseGameMode *);

    [[nodiscard]] fvec2d getStartPosition() const override;

    [[nodiscard]] fvec2d getEndPosition() const override;

protected:
    [[nodiscard]] fvec2d findDirection(double t);

    [[nodiscard]] fvec2d findNormal(double t);

    void onReset() override;

    enum class TravelDirection
    {
        Forward, Backward
    };

    void onUpdate(double delta) override;

    void onLogicUpdate(double delta) override;

    void onBegin() override;

    HitResult onFinish() override;

    void onRaise() override;

    void onDraw(Renderer &renderer) override;

    void onPress() override;

private:
    ShaderP bodyShader;
    Mesh body;

    NotOSUObjectSprite bodyTexture;
    NotOSUObjectSprite ball;
    NotOSUObjectSprite head;
    NotOSUObjectSprite headRepeat;
    NotOSUObjectSprite tail;
    NotOSUObjectSprite tailRepeat;
    NotOSUObjectSprite hitPoint;

    std::list<fvec2d> meshOutline;
    std::vector<fvec2d> meshSpine;
    std::vector<fvec2d> loops;
    std::vector<fcircle> circleMask;
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
