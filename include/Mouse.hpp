#ifndef MOUSE_HPP
#define MOUSE_HPP

#include "GlContext.hpp"

#include <glm/glm.hpp>

class Mouse {
   public:
    enum Button {
        LEFT = GLFW_MOUSE_BUTTON_1,
        RIGHT = GLFW_MOUSE_BUTTON_2,
        MIDDLE = GLFW_MOUSE_BUTTON_3,
        XBUTTON1 = GLFW_MOUSE_BUTTON_4,
        XBUTTON2 = GLFW_MOUSE_BUTTON_5,
        BUTTON_COUNT
    };

    static bool isButtonPressed(Button button);

    static glm::dvec2 getPosition();

    static void setPosition(const glm::vec2 &pos);
};

#endif
