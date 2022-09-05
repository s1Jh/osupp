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

#include "Note.hpp"

#include "Random.hpp"

#include <utility>

NS_BEGIN

void Note::onBegin()
{
    // notes are completed on the first click
    // when this gets called, the object is complete
    transferToPickup();
    wasHit = true;
}

HitResult Note::onFinish()
{
    if (!wasHit) {
        // object wasn't hit, return miss
		ctx.audio.getSFXChannel().playSound(ctx.game.getSamples().miss);
        return HitResult::Missed;
    }
	ctx.audio.getSFXChannel().playSound(ctx.game.getSamples().hit);
    // negative if hit before the start time
    double hitDelta = getTimeStarted() - getStartTime();
    // should be in the interval < -hitWindow; +hitWindow >
    int rank = 3 - Abs(int(3 * hitDelta / ctx.game.getHitWindow()));
    rank = Clamp(rank, 1, 3);
    return (HitResult) rank;
}

Note::Note(std::shared_ptr<ObjectTemplateNote> t, const HitObjectArguments& args) :
	OsuHitObject(std::move(t), args)
{
    SOF = {ctx.game.getCircleSize(), objectTemplate->position};

    const auto &skin = ctx.activeSkin;

    noteBase = skin->createObjectSprite(NOTE_BASE_SPRITE, args);
    noteOverlay = skin->createObjectSprite(NOTE_OVERLAY_SPRITE, args);
    noteUnderlay = skin->createObjectSprite(NOTE_UNDERLAY_SPRITE, args);
}

void Note::onDraw()
{
	const auto& objectTransform = getObjectTransform();

	drawApproachCircle();

    ObjectDrawInfo info{SOF, getAlpha(), objectTransform};

    ctx.gfx.draw(noteUnderlay, info);
    ctx.gfx.draw(noteBase, info);
	ctx.gfx.draw(noteOverlay, info);
}

void Note::onUpdate(double delta)
{
    noteBase.update(delta);
    noteOverlay.update(delta);
    noteUnderlay.update(delta);
}

// getEndPosition will automatically return the start position since it's not been overridden
fvec2d Note::getStartPosition() const
{ return objectTemplate->position; }

void Note::onReset()
{ wasHit = false; }

HitObjectFunction Note::getActivationFunction() const
{
	return HitObjectFunction::ButtonPressed | HitObjectFunction::CursorEnter;
}

NS_END
