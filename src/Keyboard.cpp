#include "GlContext.hpp"
#include "Keyboard.hpp"

bool Keyboard::isKeyPressed(Key key) {
    return glfwGetKey(GlContext::context(), key) == GLFW_PRESS;
}
