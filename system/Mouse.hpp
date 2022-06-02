#pragma once

#include <GL/glew.h>

#define GLFW_DLL

#include <GLFW/glfw3.h>
#include "../define.hpp"
#include "../types/Vec2.hpp"

namespace GAME_TITLE {

    struct ButtonState {
        bool released = true;
        bool pressed = false;
        bool releasing = false;
        bool pressing = false;
    };

    class Mouse {
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

}