#include "ObjectSprite.hpp"

#include "Math.hpp"
#include "Renderer.dpp"

NS_BEGIN

void ObjectSprite::update(double delta)
{
    LOG_ENTER();

    frameTimer += float(delta);

    if (frameTimer >= frameTime) {
        frameTimer = 0.0f;
        frameCounter++;
        frameCounter %= frameCount;
    }
}

void ObjectSprite::setTexture(const TextureP &textureIn)
{
    LOG_ENTER();

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

void ObjectSprite::setFPS(FPS_t fps)
{ frameTime = 1.0f / float(fps); }

void ObjectSprite::setFrameTime(float frameTimeIn)
{
    frameTime = frameTimeIn;
}

//void ObjectSprite::draw(Renderer &renderer, ObjectDrawInfo &info)
//{
//    LOG_ENTER();
//
//    if (!texture)
//        return;
//
//    auto slice = 1.0f / float(frameCount);
//    auto offset = slice * float(frameCounter);
//
//    frect clip = {{
//                      layout == AnimationLayout::Horizontal ? slice : 1.0f,
//                      layout == AnimationLayout::Horizontal ? 1.0f : slice,
//                  },
//                  {
//                      layout == AnimationLayout::Horizontal ? offset : 0.0f,
//                      layout == AnimationLayout::Horizontal ? 0.0f : offset,
//                  }};
//
//    texture->setClipArea(clip);
//
//    tint.a = info.alpha;
//    renderer.drawRect(
//        info.destination,
//        VisualAppearance{.texture = texture.get(), .fillColor = tint},
//        info.transform);
//    texture->setClipArea(UNIT_RECT<float>);
//}

void ObjectSprite::setTint(const color &tintIn)
{ tint = tintIn; }

TextureP ObjectSprite::getTexture() const
{
    return texture;
}

void detail::RenderFunctor<ObjectSprite>::operator()(Renderer &renderer,
                                                     const ObjectSprite &object,
                                                     const ObjectDrawInfo &info)
{
    if (!object.texture)
        return;

    auto slice = 1.0f / float(object.frameCount);
    auto offset = slice * float(object.frameCounter);

    frect clip = {{
                      object.layout == AnimationLayout::Horizontal ? slice : 1.0f,
                      object.layout == AnimationLayout::Horizontal ? 1.0f : slice,
                  },
                  {
                      object.layout == AnimationLayout::Horizontal ? offset : 0.0f,
                      object.layout == AnimationLayout::Horizontal ? 0.0f : offset,
                  }};

    object.texture->setClipArea(clip);

    color tint = object.tint;
    tint.a = info.alpha;

    renderer.draw(
        info.destination,
        VisualAppearance{.texture = object.texture.get(), .fillColor = tint},
        info.transform
    );

    object.texture->setClipArea(UNIT_RECT<float>);
}

BEGIN_RENDER_FUNCTOR_CONSTRUCTOR_DEFINITION(ObjectSprite)
{

}

NS_END