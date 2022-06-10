#pragma once

#include "define.hpp"
#include "Renderer.hpp"
#include "MapInfo.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "BaseHitObject.hpp"

#include <forward_list>

NS_BEGIN

    class BaseGameMode {
    public:
        using StorageT = std::forward_list<std::shared_ptr<BaseHitObject>>;

        explicit BaseGameMode();

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
        [[nodiscard]] const Mat3f& getObjectTransform() const;

    protected:
        virtual void onUpdate(double delta) = 0;

        virtual void onDraw(Renderer &) = 0;

        StorageT::iterator last;
        StorageT activeObjects;
        MapInfo *info;
        Keyboard keyboard;
        Mouse mouse;

    private:
        Mat3f transform;
        frect playField;
        double currentTime;
    };

}