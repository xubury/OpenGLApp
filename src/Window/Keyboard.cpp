#include <Window/Keyboard.hpp>
#include <Window/GLContext.hpp>

bool Keyboard::isKeyPressed(Key key) {
    return glfwGetKey(glfwGetCurrentContext(), key) == GLFW_PRESS;
}
