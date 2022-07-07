#pragma once

#include "define.hpp"

#include "EnumOperators.hpp"
#include "Font.hpp"
#include "SUI.hpp"
#include "Sprite.hpp"
#include "df2.hpp"

#include <functional>
#include <string>
#include <unordered_map>

NS_BEGIN

enum class SUIButtonFlags: uint8_t
{
    // should this button display a icon
    Icon = 1 << 0,

    // controls whether the button reacts to input
    Enabled = 1 << 1,
    Disabled = 0 << 1,

    // controls whether the button should appear locked
    // it will still call callbacks
    Locked = 1 << 2,
    Unlocked = 0 << 2,

    // controls the visibility
    Visible = 1 << 3,
    Hidden = 0 << 3,
};

ENABLE_BITMASK_OPERATORS(SUIButtonFlags)

constexpr SUIButtonFlags SUIBUTTON_DEFAULT_FLAGS = SUIButtonFlags::Enabled |
    SUIButtonFlags::Unlocked |
    SUIButtonFlags::Visible;

enum class ButtonCallbacks
{
    OnReleased,
    OnHover,
    OnHovering,
    OnPressing,
    OnPressed,
    OnReleasing
};

class SUIButton;

MAKE_CALLBACK(ButtonCallbacks, ButtonCallbacks::OnReleased, SUIButton &);
MAKE_CALLBACK(ButtonCallbacks, ButtonCallbacks::OnHover, SUIButton &);
MAKE_CALLBACK(ButtonCallbacks, ButtonCallbacks::OnHovering, SUIButton &);
MAKE_CALLBACK(ButtonCallbacks, ButtonCallbacks::OnPressing, SUIButton &);
MAKE_CALLBACK(ButtonCallbacks, ButtonCallbacks::OnPressed, SUIButton &);
MAKE_CALLBACK(ButtonCallbacks, ButtonCallbacks::OnReleasing, SUIButton &);

class SUIButton: public SUI::Element<ButtonCallbacks>
{
public:
    enum class State: uint8_t
    {
        // first we're ...
        Released,
        // then a mouse cursor starts ...
        Hovering,
        // then a button is ...
        Pressing,
        // then it's ...
        Pressed,
        // after that it's ...
        Releasing
    };

    SUIButton(const std::string &id, const drect &rect,
              const std::string &label = "",
              SUIButtonFlags flags = SUIBUTTON_DEFAULT_FLAGS);

    SUIButton(const df2 &def);

    inline State GetState()
    { return state; }

    inline std::string GetLabel()
    { return label; }

    inline Font &GetFont()
    { return font; }

    inline void SetLabel(const std::string &label)
    { label = label; }

    inline void SetReleasedSprite(Sprite spr)
    { releasedShape = spr; }

    inline void SetHoverSprite(Sprite spr)
    { hoverShape = spr; }

    inline void SetPressedSprite(Sprite spr)
    { pressedShape = spr; }

protected:
    void update(float delta) override;

    void draw(Renderer &renderer, const drect &space) override;

private:
    std::string label;

    Font font;

    Sprite icon;
    Sprite releasedShape;
    Sprite hoverShape;
    Sprite pressedShape;

    State state;
    SUIButtonFlags flags;
};
}