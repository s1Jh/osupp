#pragma once

#include "Rect.hpp"
#include "Texture.hpp"
#include "define.hpp"
#include "df2.hpp"
#include "Renderer.dpp"
#include "Shader.hpp"

#include <memory>
#include <type_traits>
#include <vector>

NS_BEGIN

class Sprite;

namespace detail
{

class BaseAnimator
{
public:
    virtual ~BaseAnimator() = default;

    virtual void applyTransform(Sprite &, [[maybe_unused]] double delta);

    virtual void firstTransform(Sprite &);

    virtual void finalTransform(Sprite &);

    virtual bool isDone();

protected:
    BaseAnimator() = default;

    void setFinished();

private:
    bool finished = false;
};
} // namespace detail

class Sprite
{
public:
    Sprite();

    explicit Sprite(const TextureP &tex);
    // Sprite ( const df2& def );

    template<class Animator, class... Args>
    void addAnimator(Args &&...args)
    {
        static_assert(
            std::is_base_of_v<detail::BaseAnimator, Animator>,
            "Added animator must be derived from the detail::BaseAnimator class.");
        auto ptr = std::make_shared<Animator>(*this, args...);
        ptr->firstTransform(*this);
        animators.push_back(std::move(ptr));
    }

    void clearAnimators();

    void update(double delta);

    void setTexture(const TextureP &tex);

    void setClipRectSize(const dsize &n);

    void setClipRectPosition(const dvec2d &n);

    void setClipRect(const drect &n);

    void setPosition(dvec2d n);

    void setSize(dsize n);

    void setRotation(float n);

    void setScale(dvec2d n);

    void setScale(double n);

    void enable();

    void disable();

    void setPivotPoint(const fvec2d &pivotPoint);

    void setTint(const color &tint);

    void setRect(const drect &tint);

    [[nodiscard]] dsize getClipRectSize() const;

    [[nodiscard]] dvec2d getClipRectPosition() const;

    [[nodiscard]] drect getClipRect() const;

    [[nodiscard]] TextureP getTexture() const;

    [[nodiscard]] dvec2d getPosition() const;

    [[nodiscard]] dsize getSize() const;

    [[nodiscard]] dvec2d getScale() const;

    [[nodiscard]] float getRotation() const;

    [[nodiscard]] bool isDisabled() const;

    [[nodiscard]] const color &getTint() const;

    [[nodiscard]] const fvec2d &getPivotPoint() const;

    [[nodiscard]] const drect &getRect() const;

private:
    std::vector<std::shared_ptr<detail::BaseAnimator>> animators;
    color tint;
    bool disabled;
    float rotation;
    fvec2d pivotPoint;
    drect position;
    drect clippingRect;
    TextureP texture;
};

BEGIN_RENDER_FUNCTOR_DECL(Sprite, const Mat3f& = MAT3_NO_TRANSFORM<float>)
        Shader shader;
END_RENDER_FUNCTOR_DECL()

NS_END
