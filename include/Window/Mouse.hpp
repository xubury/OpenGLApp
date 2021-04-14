#ifndef MOUSE_HPP
#define MOUSE_HPP

#include <glm/gtc/type_ptr.hpp>

class Mouse {
   public:
    enum Button { LEFT = 0, RIGHT = 1, MIDDLE = 2, XBUTTON1 = 3, XBUTTON2 = 4 };

    static bool isButtonPressed(Button button);

    static glm::dvec2 getPosition();

    static void setPosition(const glm::vec2 &pos);
};

#endif
