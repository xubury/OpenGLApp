#include "Core/Application.hpp"
#include "Layers/SandboxLayer.hpp"
#include "Core/Log.hpp"

int main(int, char **) {
    te::Log::init("Debug.log");

    te::Settings settings;
    settings.width = 1440;
    settings.height = 960;
    settings.title = "OpenGL";
    settings.samples = 4;
    settings.editor = true;
    settings.frameRateLimit = 120;

    te::Application app(settings);
    Ref<SandboxLayer> sandbox = te::createRef<SandboxLayer>();
    app.getEditor()->setScene(sandbox->getScene());
    app.pushLayer(sandbox);
    app.run(30);

    return 0;
}
