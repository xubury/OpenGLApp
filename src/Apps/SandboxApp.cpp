#include "Apps/SandboxApp.hpp"
#include "Layers/SandboxLayer.hpp"
#include "Layers/ShadowLayer.hpp"

namespace te {

SandboxApp::SandboxApp(Settings settings) : Application(settings) {
    pushLayer(createRef<ShadowLayer>());
    pushLayer(createRef<SandboxLayer>(settings.width, settings.height));
}

}  // namespace te
