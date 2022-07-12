#pragma once

#include "BaseHitObject.hpp"
#include "Game.hpp"
#include "Keyboard.hpp"
#include "MapInfo.hpp"
#include "Mouse.hpp"
#include "Renderer.hpp"
#include "define.hpp"

#include <forward_list>

NS_BEGIN

class BaseGameMode
{
public:
    using StorageT = std::list<std::shared_ptr<BaseHitObject>>;

    explicit BaseGameMode(Game &instance);

    virtual void update(double delta);

    virtual void draw(Renderer &);

    [[nodiscard]] double getCurrentTime() const;

    void setCurrentTime(double newTime);

    [[nodiscard]] const frect &getPlayField() const;

    void setPlayField(const frect &playField);

    [[nodiscard]] MapInfo *getMap() const;

    void setMap(MapInfo *map);

    void reset();

    [[nodiscard]] fvec2d getCursorPosition() const;

    [[nodiscard]] const Mat3f &getObjectTransform() const;

    [[nodiscard]] Game &getGame();

    [[nodiscard]] Resources &getResources();

    [[nodiscard]] float getCircleSize();

    [[nodiscard]] float getApproachTime();

    [[nodiscard]] float getFadeTime();

    [[nodiscard]] float getHitWindow();

    [[nodiscard]] SkinP getActiveSkin();

protected:
    virtual void onUpdate(double delta) = 0;

    virtual void onDraw(Renderer &) = 0;

    StorageT::iterator last;
    StorageT activeObjects;
    MapInfo *info;
    Keyboard keyboard;
    Mouse mouse;
    Game &instance;

private:
    Mat3f transform;
    frect playField;
    double currentTime;
};

NS_END