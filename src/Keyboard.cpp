#include "RenderWindow.hpp"
#include "Keyboard.hpp"

bool Keybaord::isKeyPressed(Key key) {
    return glfwGetKey(g_glContext, key) == GLFW_PRESS;
}
