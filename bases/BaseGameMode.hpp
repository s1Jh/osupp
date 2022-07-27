#pragma once

#include "define.hpp"

#include "BaseHitObject.hpp"
#include "Keyboard.hpp"
#include "MapInfo.hpp"
#include "Mouse.hpp"
#include "Renderer.dpp"
#include "Skin.hpp"

#include <list>

NS_BEGIN

class BaseGameMode
{
public:
    using StorageT = std::list<std::shared_ptr<BaseHitObject>>;

    explicit BaseGameMode();

    virtual void update(double delta);

    virtual void draw(Renderer &);

    [[nodiscard]] double getCurrentTime() const;

    void setCurrentTime(double newTime);

    [[nodiscard]] const frect &getPlayField() const;

    void setPlayField(const frect &playField);

    [[nodiscard]] MapInfoP getMap() const;

    void setMap(MapInfoP map);

    void reset();

    [[nodiscard]] fvec2d getCursorPosition() const;

    [[nodiscard]] const Mat3f &getObjectTransform() const;

    [[nodiscard]] float getCircleSize();

    [[nodiscard]] float getApproachTime();

    [[nodiscard]] float getFadeTime();

    [[nodiscard]] float getHitWindow();

protected:
    virtual void onUpdate(double delta) = 0;

    virtual void onDraw(Renderer &) = 0;

    virtual void onReset();

    StorageT::iterator last;
    StorageT activeObjects;
    MapInfoP info;
    Keyboard keyboard;
    Mouse mouse;

private:
    Mat3f transform;
    frect playField;
    double currentTime;
};

NS_END