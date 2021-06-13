#include "Apps/SandboxApp.hpp"
#include "Layers/SandboxLayer.hpp"
#include "Layers/ShadowLayer.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Component/BoundingBox.hpp"

namespace te {

SandboxApp::SandboxApp(Settings settings) : Application(settings) {
    pushLayer(createRef<ShadowLayer>());
    pushLayer(createRef<SandboxLayer>(settings.width, settings.height));
    m_scene->systems.add<BoundingBoxSystem>();
    m_scene->systems.add<PhysicsWorld>();
}

}  // namespace te
