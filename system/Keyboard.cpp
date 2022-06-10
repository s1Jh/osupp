#include "Keyboard.hpp"

#define GLFW_DLL
#include <GLFW/glfw3.h>

NS_BEGIN
KeyState Keyboard::keys[KEY_COUNT];
KeyState Keyboard::falseKey;
KeyState *Keyboard::lastKey = nullptr;
GLFWwindow *Keyboard::window = nullptr;

// FIXME: two keys are bound to apostrophe
const int Keyboard::GLFWConversionTable[KEY_COUNT] = {
        GLFW_KEY_LEFT_CONTROL, GLFW_KEY_RIGHT_CONTROL,
        GLFW_KEY_LEFT_ALT, GLFW_KEY_RIGHT_ALT,
        GLFW_KEY_LEFT_SUPER, GLFW_KEY_RIGHT_SUPER,
        GLFW_KEY_LEFT_SHIFT, GLFW_KEY_RIGHT_SHIFT,

        GLFW_KEY_ESCAPE, GLFW_KEY_BACKSPACE,

        GLFW_KEY_TAB, GLFW_KEY_UNKNOWN,
        GLFW_KEY_ENTER, GLFW_KEY_SPACE,
        GLFW_KEY_INSERT, GLFW_KEY_DELETE,
        GLFW_KEY_HOME, GLFW_KEY_END,
        GLFW_KEY_PAGE_UP, GLFW_KEY_PAGE_DOWN,
        GLFW_KEY_PRINT_SCREEN, GLFW_KEY_UNKNOWN,
        GLFW_KEY_SCROLL_LOCK, GLFW_KEY_PAUSE,
        GLFW_KEY_UNKNOWN, GLFW_KEY_MENU,
        GLFW_KEY_LEFT, GLFW_KEY_UP,
        GLFW_KEY_DOWN, GLFW_KEY_RIGHT,
        GLFW_KEY_F1, GLFW_KEY_F2,
        GLFW_KEY_F3, GLFW_KEY_F4,
        GLFW_KEY_F5, GLFW_KEY_F6,
        GLFW_KEY_F7, GLFW_KEY_F8,
        GLFW_KEY_F9, GLFW_KEY_F10,
        GLFW_KEY_F11, GLFW_KEY_F12,
        GLFW_KEY_F13, GLFW_KEY_F14,
        GLFW_KEY_F15, GLFW_KEY_F16,
        GLFW_KEY_F17, GLFW_KEY_F18,
        GLFW_KEY_F19, GLFW_KEY_F20,
        GLFW_KEY_F21, GLFW_KEY_F22,
        GLFW_KEY_F23, GLFW_KEY_F24,
        GLFW_KEY_A, GLFW_KEY_B, GLFW_KEY_C,
        GLFW_KEY_D, GLFW_KEY_E, GLFW_KEY_F,
        GLFW_KEY_G, GLFW_KEY_H, GLFW_KEY_I,
        GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_L,
        GLFW_KEY_M, GLFW_KEY_N, GLFW_KEY_O,
        GLFW_KEY_P, GLFW_KEY_Q, GLFW_KEY_R,
        GLFW_KEY_S, GLFW_KEY_T, GLFW_KEY_U,
        GLFW_KEY_V, GLFW_KEY_W, GLFW_KEY_X,
        GLFW_KEY_Y, GLFW_KEY_Z,
        GLFW_KEY_0, GLFW_KEY_1,
        GLFW_KEY_2, GLFW_KEY_3,
        GLFW_KEY_4, GLFW_KEY_5,
        GLFW_KEY_6, GLFW_KEY_7,
        GLFW_KEY_8, GLFW_KEY_9,
        GLFW_KEY_APOSTROPHE, GLFW_KEY_KP_SUBTRACT,
        GLFW_KEY_EQUAL, GLFW_KEY_LEFT_BRACKET,
        GLFW_KEY_RIGHT_BRACKET, GLFW_KEY_BACKSLASH,
        GLFW_KEY_SEMICOLON, GLFW_KEY_APOSTROPHE,
        GLFW_KEY_COMMA, GLFW_KEY_PERIOD,
        GLFW_KEY_SLASH, GLFW_KEY_NUM_LOCK,
        GLFW_KEY_KP_DIVIDE, GLFW_KEY_KP_MULTIPLY,
        GLFW_KEY_KP_SUBTRACT, GLFW_KEY_KP_ADD,
        GLFW_KEY_KP_ENTER, GLFW_KEY_KP_EQUAL,
        GLFW_KEY_PERIOD,
        GLFW_KEY_KP_0, GLFW_KEY_KP_0,
        GLFW_KEY_KP_2, GLFW_KEY_KP_3,
        GLFW_KEY_KP_4, GLFW_KEY_KP_5,
        GLFW_KEY_KP_6, GLFW_KEY_KP_7,
        GLFW_KEY_KP_8, GLFW_KEY_KP_9
};

void Keyboard::update() {
    for (int i = 0; i < KEY_COUNT; i++) {
        int code = GLFWConversionTable[i]; // the GLFW code

        bool state = code != -1 ? glfwGetKey(window, code) : false;

        auto &current = keys[i];

        // calculate the new key code
        current.code = (Key) i;

        if (i >= (unsigned int) Key::Esc) {
            // if the pressed key isn't a modifier, calculate the mods
            if (keys[(int) Key::KeyShift].pressed)
                current.code = current.code + Key::Shift;
            if (keys[(int) Key::KeyRShift].pressed)
                current.code = current.code + Key::RShift;
            if (keys[(int) Key::KeyCtrl].pressed)
                current.code = current.code + Key::Ctrl;
            if (keys[(int) Key::KeyRCtrl].pressed)
                current.code = current.code + Key::RCtrl;
            if (keys[(int) Key::KeyAlt].pressed)
                current.code = current.code + Key::Alt;
            if (keys[(int) Key::KeyRAlt].pressed)
                current.code = current.code + Key::RAlt;
            if (keys[(int) Key::KeyMeta].pressed)
                current.code = current.code + Key::Meta;
            if (keys[(int) Key::KeyRMeta].pressed)
                current.code = current.code + Key::RMeta;
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

const KeyState &Keyboard::getLastKey() {
    if (lastKey)
        return *lastKey;
    else
        return falseKey;
}

const KeyState &Keyboard::get(const Key key) {
    auto &k = keys[(int) key & 0xff];

    // Although the user can theoretically request up to
    // 64k individual "metakeys" (Shift-S, Ctrl-S, S and combinations...)
    // we only store the 255 base keys.
    // When the user then requests a key, we index with the lower 8 bits
    // and then check to make sure the metakey code is equal to the real
    // momentary code when the key is checked.
    // If not, we return a special FalseKey, a key which is always lifted.

    if (int(key) > 0xff) {
        if (k.code == key)
            return k;
        else
            return falseKey;
    } else {
        return k;
    }
}

const KeyState &Keyboard::operator[](const Key key) {
    return get(key);
}

void Keyboard::setViewport(GLFWwindow *windowIn) {
    window = windowIn;
}

NS_END