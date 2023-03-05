/*******************************************************************************
 * Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#pragma once

#include "Rect.hpp"
#include "Texture.hpp"
#include "define.hpp"
#include "df2.hpp"
#include "RenderTask.hpp"
#include "Shader.hpp"

#include <memory>
#include <type_traits>
#include <vector>

namespace PROJECT_NAMESPACE {

namespace video
{

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

    explicit Sprite(const Resource<video::Texture> &tex);
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

    void setTexture(const Resource<video::Texture> &tex);

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

    [[nodiscard]] Resource<video::Texture> getTexture() const;

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
	Resource<video::Texture> texture;
};

}

template<>
void Draw(const video::Sprite&);

}
