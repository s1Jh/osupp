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

#include "Keyboard.hpp"

#include <SDL2/SDL_keyboard.h>
#include <string>
#include <sstream>

#include "Util.hpp"

namespace PROJECT_NAMESPACE
{

namespace input
{

// FIXME: two keys are bound to apostrophe
static const int SDLConversionTable[KEY_COUNT] = {
    SDL_SCANCODE_LCTRL,
    SDL_SCANCODE_RCTRL, 
    SDL_SCANCODE_LALT,
    SDL_SCANCODE_RALT,
    SDL_SCANCODE_LGUI,
    SDL_SCANCODE_RGUI,
    SDL_SCANCODE_LSHIFT,
    SDL_SCANCODE_RSHIFT,


    SDL_SCANCODE_ESCAPE,
    SDL_SCANCODE_BACKSPACE,

    SDL_SCANCODE_TAB,
    SDL_SCANCODE_CAPSLOCK,

    SDL_SCANCODE_RETURN,
    SDL_SCANCODE_SPACE,

    SDL_SCANCODE_INSERT,
    SDL_SCANCODE_DELETE,
    SDL_SCANCODE_HOME,
    SDL_SCANCODE_END,
    SDL_SCANCODE_PAGEUP,
    SDL_SCANCODE_PAGEDOWN,
    SDL_SCANCODE_PRINTSCREEN,
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_SCROLLLOCK,
    SDL_SCANCODE_PAUSE,
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_MENU,
    SDL_SCANCODE_LEFT,
    SDL_SCANCODE_UP,
    SDL_SCANCODE_DOWN,
    SDL_SCANCODE_RIGHT,
    SDL_SCANCODE_F1,
    SDL_SCANCODE_F2,
    SDL_SCANCODE_F3,
    SDL_SCANCODE_F4,
    SDL_SCANCODE_F5,
    SDL_SCANCODE_F6,
    SDL_SCANCODE_F7,
    SDL_SCANCODE_F8,
    SDL_SCANCODE_F9,
    SDL_SCANCODE_F10,
    SDL_SCANCODE_F11,
    SDL_SCANCODE_F12,
    SDL_SCANCODE_F13,
    SDL_SCANCODE_F14,
    SDL_SCANCODE_F15,
    SDL_SCANCODE_F16,
    SDL_SCANCODE_F17,
    SDL_SCANCODE_F18,
    SDL_SCANCODE_F19,
    SDL_SCANCODE_F20,
    SDL_SCANCODE_F21,
    SDL_SCANCODE_F22,
    SDL_SCANCODE_F23,
    SDL_SCANCODE_F24,
    SDL_SCANCODE_A,
    SDL_SCANCODE_B,
    SDL_SCANCODE_C,
    SDL_SCANCODE_D,
    SDL_SCANCODE_E,
    SDL_SCANCODE_F,
    SDL_SCANCODE_G,
    SDL_SCANCODE_H,
    SDL_SCANCODE_I,
    SDL_SCANCODE_J,
    SDL_SCANCODE_K,
    SDL_SCANCODE_L,
    SDL_SCANCODE_M,
    SDL_SCANCODE_N,
    SDL_SCANCODE_O,
    SDL_SCANCODE_P,
    SDL_SCANCODE_Q,
    SDL_SCANCODE_R,
    SDL_SCANCODE_S,
    SDL_SCANCODE_T,
    SDL_SCANCODE_U,
    SDL_SCANCODE_V,
    SDL_SCANCODE_W,
    SDL_SCANCODE_X,
    SDL_SCANCODE_Y,
    SDL_SCANCODE_Z,
    SDL_SCANCODE_0,
    SDL_SCANCODE_1,
    SDL_SCANCODE_2,
    SDL_SCANCODE_3,
    SDL_SCANCODE_4,
    SDL_SCANCODE_5,
    SDL_SCANCODE_6,
    SDL_SCANCODE_7,
    SDL_SCANCODE_8,
    SDL_SCANCODE_9,
    SDL_SCANCODE_APOSTROPHE,
    SDL_SCANCODE_MINUS,
    SDL_SCANCODE_EQUALS,
    SDL_SCANCODE_LEFTBRACKET,
    SDL_SCANCODE_RIGHTBRACKET,
    SDL_SCANCODE_BACKSLASH,
    SDL_SCANCODE_SEMICOLON,
    SDL_SCANCODE_APOSTROPHE,
    SDL_SCANCODE_COMMA,
    SDL_SCANCODE_PERIOD,
    SDL_SCANCODE_SLASH,
    SDL_SCANCODE_NUMLOCKCLEAR,
    SDL_SCANCODE_KP_DIVIDE,
    SDL_SCANCODE_KP_MULTIPLY,
    SDL_SCANCODE_KP_MINUS,
    SDL_SCANCODE_KP_PLUS,
    SDL_SCANCODE_KP_ENTER,
    SDL_SCANCODE_KP_EQUALS,
    SDL_SCANCODE_PERIOD,
    SDL_SCANCODE_KP_0,
    SDL_SCANCODE_KP_0,
    SDL_SCANCODE_KP_2,
    SDL_SCANCODE_KP_3,
    SDL_SCANCODE_KP_4,
    SDL_SCANCODE_KP_5,
    SDL_SCANCODE_KP_6,
    SDL_SCANCODE_KP_7,
    SDL_SCANCODE_KP_8,
    SDL_SCANCODE_KP_9};

void Keyboard::update()
{
//    auto &ctx = GetContext();
//    auto *handle = ctx.gfx.getWindow().getHandle();
//    if (handle == nullptr) {
//        return;
//    }
    int keyCount;
    auto* kbd = SDL_GetKeyboardState(&keyCount);
    for (unsigned int i = 0; i < KEY_COUNT; i++) {
        int scanCode = SDLConversionTable[i]; // the GLFW code

        bool state = false;
        state = kbd[scanCode % keyCount];

        auto &current = keys[i];

        // calculate the new key code
        current.code = (Key) i;

        if (i >= (unsigned int) Key::ESC) {
            // if the pressed key isn't a modifier, calculate the mods
            if (keys[(int) Key::KEY_SHIFT].pressed) {
                current.code = current.code + Key::SHIFT;
            }
            if (keys[(int) Key::KEY_RSHIFT].pressed) {
                current.code = current.code + Key::RSHIFT;
            }
            if (keys[(int) Key::KEY_CTRL].pressed) {
                current.code = current.code + Key::CTRL;
            }
            if (keys[(int) Key::KEY_RCTRL].pressed) {
                current.code = current.code + Key::RCTRL;
            }
            if (keys[(int) Key::KEY_ALT].pressed) {
                current.code = current.code + Key::ALT;
            }
            if (keys[(int) Key::KEY_RALT].pressed) {
                current.code = current.code + Key::RALT;
            }
            if (keys[(int) Key::KEY_META].pressed) {
                current.code = current.code + Key::META;
            }
            if (keys[(int) Key::KEY_RMETA].pressed) {
                current.code = current.code + Key::RMETA;
            }
        }

        // onFinish previous pressing/releasing ops
        if (current.releasing) {
            current.releasing = false;
            current.released = true;
        }
        if (current.pressing) {
            current.pressing = false;
            current.pressed = true;
        }

        // queue the new ones
        if (state && current.released) {
            current.released = false;
            current.pressing = true;
            lastKey = &keys[i];
        }
        if (!state && current.pressed) {
            current.pressed = false;
            current.releasing = true;
        }
    }
}

const KeyState &Keyboard::getLastKey() const
{
    if (lastKey) {
        return *lastKey;
    } else {
        return falseKey;
    }
}

const KeyState &Keyboard::get(const Key key) const
{
    auto &k = keys[(int) key & 0xff];

    // Although the user can theoretically request up to
    // 64k individual "metakeys" (Shift-S, Ctrl-S, S and combinations...)
    // we only store the 255 base keys.
    // When the user then requests a key, we index with the lower 8 bits
    // and then check to make sure the metakey code is equal to the real
    // momentary code when the key is checked.
    // If not, we return a special FalseKey, a key which is always lifted.

    if (int(key) > 0xff) {
        if (k.code == key) {
            return k;
        } else {
            return falseKey;
        }
    } else {
        return k;
    }
}

const KeyState &Keyboard::operator[](const Key key) const
{ return get(key); }

}

template<>
std::optional<input::Key> FromString<input::Key>(const std::string &str)
{
    auto parts = GetCharacterSeparatedValues(str, '+');
    auto ret = static_cast<input::Key>(0);
    bool keyFound = false;
    for (auto &part : parts) {
        Trim(part);
        UpperCase(part);

        if (part == "CTRL") {
            ret |= input::Key::CTRL;
        }
        if (part == "RCTRL") {
            ret |= input::Key::RCTRL;
        }
        if (part == "ALT") {
            ret |= input::Key::ALT;
        }
        if (part == "RALT") {
            ret |= input::Key::RALT;
        }
        if (part == "SHIFT") {
            ret |= input::Key::SHIFT;
        }
        if (part == "RSHIFT") {
            ret |= input::Key::RSHIFT;
        }
        if (part == "META") {
            ret |= input::Key::META;
        }
        if (part == "RMETA") {
            ret |= input::Key::RMETA;
        }

        if (!keyFound) {
            // Assume it's an actual key and
            // not a modifier.
            // Use the keyFound to indicate that a key name
            // has already been located in the key combination,
            // otherwise we could end up OR-ing multiple
            // keycodes together.
#define KEY(k) if (part == #k) { ret |= input::Key::k; keyFound = true; }
            KEY_LIST
#undef KEY
        }
    }

    if (keyFound) {
        return ret;
    }
    return {};
}

template<>
std::string ToString<input::Key>(const input::Key &key)
{
    std::stringstream os;

    if ((unsigned int) key & (unsigned int) input::Key::CTRL) {
        os << "Ctrl + ";
    }
    if ((unsigned int) key & (unsigned int) input::Key::RCTRL) {
        os << "RCtrl + ";
    }
    if ((unsigned int) key & (unsigned int) input::Key::ALT) {
        os << "Alt + ";
    }
    if ((unsigned int) key & (unsigned int) input::Key::RALT) {
        os << "RAlt + ";
    }
    if ((unsigned int) key & (unsigned int) input::Key::SHIFT) {
        os << "Shift + ";
    }
    if ((unsigned int) key & (unsigned int) input::Key::RSHIFT) {
        os << "RShift + ";
    }
    if ((unsigned int) key & (unsigned int) input::Key::META) {
        os << "Meta + ";
    }
    if ((unsigned int) key & (unsigned int) input::Key::RMETA) {
        os << "RMeta + ";
    }

    input::Key code = input::Key((int) key & 0xff);
    switch (code) {
#define KEY(name)                                                              \
  case input::Key::name:                                                              \
    os << #name;                                                               \
    break;
        KEY_LIST
#undef KEY
        default: os << "Unknown (" << std::to_string((int) code) << ')';
            break;
    }
    return os.str();
}

}
