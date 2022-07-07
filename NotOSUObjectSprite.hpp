#pragma once

#include "define.hpp"

#include "Renderable.hpp"
#include "Texture.hpp"
#include "Types.hpp"

NS_BEGIN

struct NotOSUObjectDrawInfo
{
    frect destination = UNIT_RECT<float>;
    float alpha = 1.0f;
    const Mat3f &transform = MAT3_NO_TRANSFORM<float>;
};

class NotOSUObjectSprite: public detail::Renderable<NotOSUObjectDrawInfo>
{
public:
    NotOSUObjectSprite() = default;

    void draw(Renderer &renderer, NotOSUObjectDrawInfo &) override;
    void update(double delta);

    void setTexture(const TextureP &texture);
    void setFPS(FPS_t fps);
    void setFrameTime(float frameTime);

private:
    color tint;

public:
    void setTint(const color &tint);

private:
    TextureP texture;
    AnimationLayout layout = AnimationLayout::Horizontal;
    float frameTime = 0.0f;
    float frameTimer = 0.0f;
    unsigned int frameCount = 1;
    unsigned int frameCounter = 0;
};

NS_END
