#pragma once

#include "Vec2.hpp"
#include "define.hpp"

typedef struct GLFWwindow GLFWwindow;

NS_BEGIN

struct ButtonState
{
    bool released: 1 = true;
    bool pressed: 1 = false;
    bool releasing: 1 = false;
    bool pressing: 1 = false;
};

class Mouse
{
public:
    static const ButtonState &left();

    static const ButtonState &middle();

    static const ButtonState &right();

    static fvec2d position();

    static void setViewport(GLFWwindow *n);

    static void update();

protected:
    static GLFWwindow *parentViewport;
    static ButtonState buttons[3];
};

NS_END