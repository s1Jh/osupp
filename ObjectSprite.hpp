#pragma once

#include "define.hpp"

#include "Renderer.hpp"
#include "Texture.hpp"
#include "Types.hpp"

NS_BEGIN

struct ObjectDrawInfo
{
    frect destination = UNIT_RECT<float>;
    float alpha = 1.0f;
    const Mat3f &transform = MAT3_NO_TRANSFORM<float>;
};

class ObjectSprite
{
public:
    BEFRIEND_RENDER_FUNCTOR(ObjectSprite)

    ObjectSprite() = default;

    void update(double delta);

    void setTexture(const TextureP &texture);
    void setFPS(FPS_t fps);
    void setFrameTime(float frameTime);
    void setTint(const color &tint);

    [[nodiscard]] TextureP getTexture() const;

private:
    color tint;
    TextureP texture;
    AnimationLayout layout = AnimationLayout::Horizontal;
    float frameTime = 0.0f;
    float frameTimer = 0.0f;
    unsigned int frameCount = 1;
    unsigned int frameCounter = 0;
};

BEGIN_RENDER_FUNCTOR_DECL(ObjectSprite, const ObjectDrawInfo&)
END_RENDER_FUNCTOR_DECL()

NS_END
