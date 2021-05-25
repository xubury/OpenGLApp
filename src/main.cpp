#include <Game.hpp>

int main() {
    Settings settings;
    settings.width = 1440;
    settings.height = 960;
    settings.title = "OpenGL";
    settings.samples = 4;
    settings.editor = true;

    Game window(settings);
    window.run(30);

    return 0;
}
