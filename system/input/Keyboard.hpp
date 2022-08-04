#pragma once

#include "define.hpp"

typedef struct GLFWwindow GLFWwindow;

#include <iostream>

NS_BEGIN

/**
 * The key enum defines basic codes for valid _physical_ keys
 * on a standard US QWERTY layout keyboard.
 *
 * This code includes all possible permutations of modifier keys.
 * This means that Shift+C and C are unique keys. Getter methods
 * of keyboard classes will OR any active modifier keys onto the
 * key's code when returning the KeyState.
 *
 * Modifier keys are also accessible as unique keys.
 */
#define KEY_LIST                                                                   \
  KEY(KeyCtrl)                                                                     \
  KEY(KeyRCtrl) KEY(KeyAlt) KEY(KeyRAlt) KEY(KeyMeta) KEY(KeyRMeta) KEY(           \
      KeyShift) KEY(KeyRShift)                                                     \
                                                                                   \
      KEY(Esc) KEY(BackSpace) KEY(Tab) KEY(CapsLock) KEY(Enter) KEY(               \
          Space) KEY(Insert) KEY(Delete) KEY(Home) KEY(End) KEY(PageUp)            \
          KEY(PageDown) KEY(PrtScr) KEY(SysRq) KEY(ScrollLock) KEY(Pause) KEY(     \
              Break) KEY(Menu)                                                     \
                                                                                   \
              KEY(Left) KEY(Up) KEY(Down) KEY(Right)                               \
                                                                                   \
                  KEY(F1) KEY(F2) KEY(F3) KEY(F4) KEY(F5) KEY(F6) KEY(F7) KEY(     \
                      F8) KEY(F9) KEY(F10) KEY(F11) KEY(F12) KEY(F13) KEY(F14)     \
                      KEY(F15) KEY(F16) KEY(F17) KEY(F18) KEY(F19) KEY(            \
                          F20) KEY(F21) KEY(F22) KEY(F23) KEY(F24)                 \
                                                                                   \
                          KEY(A) KEY(B) KEY(C) KEY(D) KEY(E) KEY(F) KEY(           \
                              G) KEY(H) KEY(I) KEY(J) KEY(K) KEY(L) KEY(M)         \
                              KEY(N) KEY(O) KEY(P) KEY(Q) KEY(R) KEY(S) KEY(       \
                                  T) KEY(U) KEY(V) KEY(W) KEY(X) KEY(Y) KEY(Z)     \
                                                                                   \
                                  KEY(K0) KEY(K1) KEY(K2) KEY(K3) KEY(K4) KEY(     \
                                      K5) KEY(K6) KEY(K7) KEY(K8) KEY(K9)          \
                                                                                   \
                                      KEY(Tilde) KEY(Minus) KEY(Equals) KEY(       \
                                          LeftBracket) KEY(RightBracket)           \
                                          KEY(BackSlash) KEY(Semicolon) KEY(       \
                                              Apostrophe) KEY(Comma)               \
                                              KEY(Period) KEY(Slash) KEY(          \
                                                  NumLock)                         \
                                                                                   \
                                                  KEY(KPDivide) KEY(               \
                                                      KPMultiply) KEY(KPSubtract)  \
                                                      KEY(KPAdd) KEY(KPEnter) KEY( \
                                                          KPEquals) KEY(KPDecimal) \
                                                                                   \
                                                          KEY(KP0) KEY(KP1) KEY(   \
                                                              KP2) KEY(KP3)        \
                                                              KEY(KP4) KEY(        \
                                                                  KP5) KEY(KP6)    \
                                                                  KEY(KP7) KEY(    \
                                                                      KP8)         \
                                                                      KEY(KP9)

enum class Key
{

#define KEY(name) name,

    KEY_LIST

#undef KEY

    Unknown,

    // Modifier keys
    Ctrl = 1 << 8,
    RCtrl = 1 << 9,

    Alt = 1 << 10,
    RAlt = 1 << 11,

    Meta = 1 << 12,
    RMeta = 1 << 13,

    Shift = 1 << 14,
    RShift = 1 << 15,
};

std::ostream &operator<<(std::ostream &os, const Key &key);

#ifdef IMPLEMENTATION
std::ostream &operator<<(std::ostream &os, const Key &key) {
  if ((unsigned int)key & (unsigned int)Key::Ctrl)
    os << "Ctrl + ";
  if ((unsigned int)key & (unsigned int)Key::RCtrl)
    os << "RCtrl + ";
  if ((unsigned int)key & (unsigned int)Key::Alt)
    os << "Alt + ";
  if ((unsigned int)key & (unsigned int)Key::RAlt)
    os << "RAlt + ";
  if ((unsigned int)key & (unsigned int)Key::Shift)
    os << "Shift + ";
  if ((unsigned int)key & (unsigned int)Key::RShift)
    os << "RShift + ";
  if ((unsigned int)key & (unsigned int)Key::Meta)
    os << "Meta + ";
  if ((unsigned int)key & (unsigned int)Key::RMeta)
    os << "RMeta + ";

  Key code = Key((int)key & 0xff);
  switch (code) {
#define KEY(name)                                                              \
  case Key::name:                                                              \
    os << #name;                                                               \
    break;
    KEY_LIST
#undef KEY
  default:
    os << "Unknown (" << std::to_string((int)code) << ')';
    break;
  }
  return os;
}
#endif

/**
 * Total number of different recognizable keys.
 */
constexpr unsigned int KEY_COUNT = (unsigned int) Key::Unknown;

constexpr Key operator+(const Key &mod, const Key &key)
{
    return static_cast<Key>(static_cast<unsigned int>(mod) |
        static_cast<unsigned int>(key));
}

/**
 * All the information available about a key.
 */
struct KeyState
{
    Key code = Key::Unknown;
    char ascii = 0;

    bool released: 1 = true;
    bool pressed: 1 = false;
    bool releasing: 1 = false;
    bool pressing: 1 = false;
};

class Keyboard
{
public:
    static void setViewport(GLFWwindow *window);

    static void update();

    static const KeyState &getLastKey();

    static const KeyState &get(Key key);

    const KeyState &operator[](Key key);

protected:
    static const int GLFWConversionTable[KEY_COUNT];
    static GLFWwindow *window;
    static KeyState *lastKey;
    static KeyState falseKey;
    static KeyState keys[KEY_COUNT];
};

NS_END