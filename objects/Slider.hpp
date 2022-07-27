#pragma once

#include "HitObject.tpp"
#include "SliderTemplate.hpp"
#include "define.hpp"

NS_BEGIN

constexpr double SLIDER_DIRECTION_EPSILON = 0.01;

constexpr double SLIDER_ANGLE_OPT_THRESHOLD = 0.05;

constexpr double SLIDER_DISTANCE_OPT_THRESHOLD = 0.1;

constexpr unsigned int SLIDER_STEPS_PER_CURVE_UNIT = 10;

class Slider: public HitObject<ObjectTemplateSlider>
{
public:
    explicit Slider(std::shared_ptr<ObjectTemplateSlider>, BaseGameMode &);

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

    ObjectSprite bodyTexture;
    ObjectSprite ball;
    ObjectSprite head;
    ObjectSprite headRepeat;
    ObjectSprite tail;
    ObjectSprite tailRepeat;
    ObjectSprite hitPoint;

    std::list<fvec2d> meshOutline;
    std::vector<fvec2d> meshSpine;
    std::vector<fvec2d> loops;
    std::vector<fcircle> circleMask;

    // the interpolated path
    SliderPathT interpolatedPath;
    Curve<SliderPathT::iterator> curve;

    // cached variables
    float endBumperAngle;
    float startBumperAngle;

    // state
    TravelDirection currentDirection;
    bool active;
    bool broken;
    bool started;
    unsigned int repeatsLeft;
    double progression;
    double curvePosition;
};

NS_END
