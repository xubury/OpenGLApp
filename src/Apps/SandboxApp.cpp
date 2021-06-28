#include "Apps/SandboxApp.hpp"
#include "Layers/SandboxLayer.hpp"
#include "Layers/ShadowLayer.hpp"
#include "Layers/SkyboxLayer.hpp"
#include "Layers/GBufferLayer.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Component/BoundingBox.hpp"

namespace te {

SandboxApp::SandboxApp(Settings settings) : Application(settings) {
    pushLayer(createRef<ShadowLayer>());
    pushLayer(createRef<GBufferLayer>(settings.width, settings.height,
                                      settings.samples));
    pushLayer(createRef<SandboxLayer>(settings.width, settings.height));
    pushLayer(createRef<SkyboxLayer>());
    m_scene->systems.add<BoundingBoxSystem>();
    m_scene->systems.add<PhysicsWorld>();
}

}  // namespace te
