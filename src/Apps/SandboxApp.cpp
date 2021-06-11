#include "Apps/SandboxApp.hpp"
#include "Layers/SandboxLayer.hpp"

SandboxApp::SandboxApp(te::Settings settings) : te::Application(settings) {
    te::Ref<SandboxLayer> sandbox =
        te::createRef<SandboxLayer>(settings.width, settings.height);
    getEditor()->setScene(sandbox->getScene());
    pushLayer(sandbox);
}
