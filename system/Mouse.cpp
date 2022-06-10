#include "Mouse.hpp"

#include "Math.hpp"

#define GLFW_DLL
#include <GLFW/glfw3.h>

NS_BEGIN

GLFWwindow *Mouse::parentViewport = nullptr;
ButtonState Mouse::buttons[3];

const ButtonState &Mouse::left() {
    return buttons[0];
}

const ButtonState &Mouse::middle() {
    return buttons[2];
}

const ButtonState &Mouse::right() {
    return buttons[1];
}

fvec2d Mouse::position() {
    dvec2d ret;
    glfwGetCursorPos(parentViewport, &ret.x, &ret.y);

    isize size;
    glfwGetWindowSize(parentViewport, &size.w, &size.h);

    auto shorter = double(Min(size.w, size.h)) / 2.0;

    ret -= dvec2d(size / 2);
    ret /= dvec2d{shorter, shorter};
    ret.y *= -1.0f;

    return ret;
}

void Mouse::setViewport(GLFWwindow *n) {
    parentViewport = n;
}

void Mouse::update() {
    for (size_t i = 0; i < 3; i++) {
        bool state = glfwGetMouseButton(parentViewport, GLFW_MOUSE_BUTTON_1 + i);

        auto &button = buttons[i];

        // onFinish previous pressing/releasing ops
        if (button.releasing) {
            button.releasing = false;
            button.released = true;
        }
        if (button.pressing) {
            button.pressing = false;
            button.pressed = true;
        }

        // queue the new ones
        if (state && button.released) {
            button.released = false;
            button.pressing = true;
        }
        if (!state && button.pressed) {
            button.pressed = false;
            button.releasing = true;
        }
    }
}

NS_END