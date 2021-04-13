#include "GlContext.hpp"
#include "Keyboard.hpp"

bool Keybaord::isKeyPressed(Key key) {
    return glfwGetKey(GlContext::context(), key) == GLFW_PRESS;
}
