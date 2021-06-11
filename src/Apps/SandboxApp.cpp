#include "Apps/SandboxApp.hpp"
#include "Layers/SandboxLayer.hpp"

namespace te {

SandboxApp::SandboxApp(Settings settings) : Application(settings) {
    Ref<SandboxLayer> sandbox =
        createRef<SandboxLayer>(settings.width, settings.height);
    pushLayer(sandbox);
}

}  // namespace te
