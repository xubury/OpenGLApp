#include <Window/Mouse.hpp>
#include <Window/GLContext.hpp>
#include <GLFW/glfw3.h>

namespace te {

bool Mouse::isButtonPressed(Button button) {
    return glfwGetMouseButton(glfwGetCurrentContext(), button) == GLFW_PRESS;
}

glm::dvec2 Mouse::getPosition() {
    glm::dvec2 pos;
    glfwGetCursorPos(glfwGetCurrentContext(), &pos.x, &pos.y);
    return pos;
}

void Mouse::setPosition(const glm::vec2 &pos) {
    glfwSetCursorPos(glfwGetCurrentContext(), pos.x, pos.y);
}

}

