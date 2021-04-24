#include <Game.hpp>
#include <iostream>

int main() {
    Game window(800, 600, "OpenGL");
    window.run(30);
    return 0;
}
