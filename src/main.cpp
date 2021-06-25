#include "Core/Log.hpp"
#include "Apps/SandboxApp.hpp"

int main(int, char **) {
    te::Log::init("Debug.log");

    te::Settings settings;
    settings.width = 1440;
    settings.height = 960;
    settings.title = "OpenGL";
    settings.samples = 4;
    settings.editor = true;
    settings.frameRateLimit = 0;

    te::SandboxApp app(settings);
    app.run(30);

    return 0;
}
