#include "Keyboard.hpp"

bool Keyboard::isKeyPressed(Key key) {
    return glfwGetKey(glfwGetCurrentContext(), key) == GLFW_PRESS;
}
