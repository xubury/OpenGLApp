#include <Window/Keyboard.hpp>
#include <Window/GLContext.hpp>
#include <GLFW/glfw3.h>

bool Keyboard::isKeyPressed(Key key) {
    return glfwGetKey(glfwGetCurrentContext(), key) == GLFW_PRESS;
}
