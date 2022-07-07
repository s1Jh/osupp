#include "NotOSUObjectSprite.hpp"

#include "Math.hpp"
#include "Renderer.hpp"

NS_BEGIN

void NotOSUObjectSprite::update(double delta)
{
    frameTimer += float(delta);

    if (frameTimer >= frameTime) {
        frameTimer = 0.0f;
        frameCounter++;
        frameCounter %= frameCount;
    }
}

void NotOSUObjectSprite::setTexture(const TextureP &textureIn)
{
    if (textureIn) {
        texture = textureIn;
        frameTimer = 0;
        frameCounter = 0;

        auto longerSide = (float) Max(texture->getHeight(), texture->getWidth());
        auto shorterSide = (float) Min(texture->getHeight(), texture->getWidth());
        layout = texture->getWidth() >= texture->getHeight()
                 ? AnimationLayout::Horizontal
                 : AnimationLayout::Vertical;
        frameCount = Max(int(longerSide / shorterSide), 1);

        // advance the animation by one frame upon construction
        // to ensure a proper clip is set up before the texture is used
        update(frameTime);
    }
}

void NotOSUObjectSprite::setFPS(FPS_t fps)
{ frameTime = 1.0f / float(fps); }

void NotOSUObjectSprite::setFrameTime(float frameTimeIn)
{
    frameTime = frameTimeIn;
}

void NotOSUObjectSprite::draw(Renderer &renderer, NotOSUObjectDrawInfo &info)
{
    if (!texture)
        return;

    auto slice = 1.0f / float(frameCount);
    auto offset = slice * float(frameCounter);

    frect clip = {{
                      layout == AnimationLayout::Horizontal ? slice : 1.0f,
                      layout == AnimationLayout::Horizontal ? 1.0f : slice,
                  },
                  {
                      layout == AnimationLayout::Horizontal ? offset : 0.0f,
                      layout == AnimationLayout::Horizontal ? 0.0f : offset,
                  }};

    texture->setClipArea(clip);

    tint.a = info.alpha;
    renderer.drawRect(
        info.destination,
        VisualAppearance{.texture = texture.get(), .fillColor = tint},
        info.transform);
    texture->setClipArea(UNIT_RECT<float>);
}

void NotOSUObjectSprite::setTint(const color &tintIn)
{ tint = tintIn; }

NS_END