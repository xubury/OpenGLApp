#include "Game.hpp"
#include "Core/Log.hpp"

int main() {
    te::Log::init();

    Settings settings;
    settings.width = 1440;
    settings.height = 960;
    settings.title = "OpenGL";
    settings.samples = 4;
    settings.editor = true;
    settings.frameRateLimit = 120;

    Game window(settings);
    window.run(30);

    return 0;
}
