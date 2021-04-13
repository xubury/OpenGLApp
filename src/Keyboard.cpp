#include "RenderWindow.hpp"
#include "Keyboard.hpp"

bool Keyboard::isKeyPressed(Key key) {
    return glfwGetKey(RenderWindow::context(), key) == GLFW_PRESS;
}
