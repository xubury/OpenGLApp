#include <Window/Keyboard.hpp>
#include <Window/GLContext.hpp>
#include <GLFW/glfw3.h>

namespace te {

bool Keyboard::isKeyPressed(Key key) {
    return glfwGetKey(glfwGetCurrentContext(), key) == GLFW_PRESS;
}

}  // namespace te
