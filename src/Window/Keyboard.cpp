#include <Window/Keyboard.hpp>
#include <Window/GlContext.hpp>

bool Keyboard::isKeyPressed(Key key) {
    return glfwGetKey(glfwGetCurrentContext(), key) == GLFW_PRESS;
}
