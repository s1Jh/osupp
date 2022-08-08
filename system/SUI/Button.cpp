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

#include "Button.hpp"

#include "Math.hpp"

NS_BEGIN

SUIButton::SUIButton(const std::string &id, const drect &rect,
                     const std::string &label, SUIButtonFlags flags)
    : Element(id, rect), label(label), font(nullptr), flags(flags)
{
    /*
    font = Font(AssetManager::GetSymLink<AssetFont>(SL_DEFAULT_FONT));
    font.setSize(rect.size.h / 2);
    pressedShape = Sprite(AssetManager::GetSymLink<Texture>("pressed.bmp"));
    hoverShape = Sprite(AssetManager::GetSymLink<Texture>("hover.bmp"));
    releasedShape = Sprite(AssetManager::GetSymLink<Texture>("released.bmp"));*/
}

SUIButton::SUIButton(const df2 &def)
    : Element(def.name(), drect{}), font(nullptr)
{
    drect bounds = {dsize(def["size"].vec({128, 64})), def["position"].vec()};
    setBounds(bounds);

    flags = (SUIButtonFlags) def["flags"].integer((int) SUI_DEFAULT_FLAGS);
    label = def["label"].str("");
    /*font =
    Font(AssetManager::GetSymLink<AssetFont>(def["font"]["name"].str(SL_DEFAULT_FONT)));
    font.setSize(def["font"]["size"].integer(11));

    auto SetSprite = [&](Sprite &spr, const std::string &name) {
        const auto &shape = def["shape"];

        if (shape[name].GetType() == DefinitionFile::Type::String)
            spr = Sprite(AssetManager::GetSymLink<Texture>(shape[name].Str()));
        else
            spr = Sprite(shape[name]);
    };

    SetSprite(pressedShape, "pressed");
    SetSprite(hoverShape, "hover");
    SetSprite(releasedShape, "released");*/

    /*auto LinkCallback = [&](const std::string &name, SUI::CallbackFunction
    &func) { if (function_map.find(def["callbacks"][name].Str()) !=
    function_map.end()) { Log::Debug("[UI PARSE] Matching function ", name); func
    = function_map[def["callbacks"][name].Str()];
        }
    };

    LinkCallback("on_hover", m_pOnHover);
    LinkCallback("on_pressing", m_pOnPress);
    LinkCallback("on_releasing", m_pOnRelease);
    LinkCallback("on_pressed", m_pOnHeld);
    LinkCallback("on_released", m_pOnReleased);*/
}

void SUIButton::draw(Renderer &renderer, const drect &space)
{
    /*Sprite *selected_spr = nullptr;

    switch (state) {
        case State::Hovering:
            selected_spr = &m_HoverShape;
            break;

        case State::Pressing:
        case State::Pressed:
            selected_spr = &m_PressedShape;
            break;

        case State::Releasing:
        case State::Released:
            selected_spr = &m_ReleasedShape;
            break;

        default:
            return;
    }

    selected_spr->setSize(space.size);

    DrawSprite(*selected_spr, space.position, selected_spr->getScale(),
    selected_spr->getRotation());

    std::string selected = label == "" ? getName() : label;

    dsize projected = font.PreviewSize(selected);
    dvec2d position = (space.position + dvec2d(space.size / 2)) - dvec2d(projected
    / 2);

    DrawText(font, selected, position);*/
}

void SUIButton::update(float delta)
{
    // test if a cursor intersects with the button
    if (Intersects(getBounds(), Mouse::position())) {
        // cursor is hovering
        state = State::Hovering;
        invokeCallback<ButtonCallbacks::OnHovering>(*this);

        // inherit the button state from the mouse, if anything is pressed
        if (Mouse::left().pressed) {
            state = State::Pressed;
        }
        if (Mouse::left().releasing) {
            state = State::Releasing;
        }
        if (Mouse::left().pressing) {
            state = State::Pressing;
        }
    }
    else {
        // if not, there is only one state we can be in
        state = State::Released;
    }

    // invoke the callback appropriate to the current state
    /*switch (state) {
        case State::Released:
            if (m_pOnReleased) {
                m_pOnReleased({});
            }
            break;
        case State::Hovering:
            if (m_pOnHover) {
                m_pOnHover({});
            }
            break;
        case State::Pressing:
            if (m_pOnPress) {
                m_pOnPress({});
            }
            break;
        case State::Pressed:
            if (m_pOnHeld) {
                m_pOnHeld({});
            }
            break;
        case State::Releasing:
            if (m_pOnRelease) {
                m_pOnRelease({});
            }
            break;
        default:
            break;
    };*/
}
}