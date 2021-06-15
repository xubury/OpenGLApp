#include "Layers/ShadowLayer.hpp"
#include "Apps/Application.hpp"
#include "Graphic/Renderer.hpp"
#include "Entity/Light.hpp"
#include "Component/ShadowMap.hpp"

namespace te {

const uint32_t SHADOW_MAP_WIDTH = 4096;
const uint32_t SHADOW_MAP_HEIGHT = 4096;

ShadowLayer::ShadowLayer() : Layer("Shadow layer") {
    FrameBufferSpecification spec;
    spec.width = SHADOW_MAP_WIDTH;
    spec.height = SHADOW_MAP_HEIGHT;
    spec.attachmentsSpecs = {{FramebufferTextureFormat::RGB},
                             {FramebufferTextureFormat::DEPTH32}};
    m_framebuffer = createRef<FrameBuffer>(spec, false);

    const char *shadowVertex = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        struct DirLight {
            vec3 direction;
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        };

        layout (std140) uniform Light
        {
            mat4 uLightSpaceMatrix;
            DirLight uDirLight;
        };
        uniform mat4 uModel;
        out float zValue;
        void main() {
            gl_Position = uLightSpaceMatrix * uModel * vec4(aPos, 1.0);
            zValue = gl_Position.z / gl_Position.w;
            zValue = smoothstep(0, 1.0, zValue);
        }

    )";
    const char *shadowFragment = R"(
        #version 330 core
        out vec4 fragColor;
        in float zValue;
        void main() {
            fragColor = vec4(vec3(zValue), 1.0);
        };
    )";
    m_shader = createRef<Shader>();
    m_shader->compile(shadowVertex, shadowFragment);
}

void ShadowLayer::onUpdate(const Time &) {
    Ref<SceneManager<EntityBase>> scene =
        Application::instance().getActiveScene();
    ShadowMap::Handle shadowMap;
    scene->entities.getByComponents(shadowMap).begin();
    if (!shadowMap.isValid()) return;

    Ref<Camera> cam = Application::instance().getMainCamera();
    shadowMap->computeLightSpaceMatrix(cam);
}

void ShadowLayer::onRender() {
    Ref<SceneManager<EntityBase>> scene =
        Application::instance().getActiveScene();
    ShadowMap::Handle shadowMap;
    scene->entities.getByComponents(shadowMap).begin();
    if (!shadowMap.isValid()) return;

    Renderer::beginShadowCast(shadowMap.get(), m_framebuffer);
    std::size_t size = scene->entities.size();
    for (std::size_t i = 0; i < size; ++i) {
        scene->entities.get(i)->draw(*m_shader);
    }
    Renderer::endShadowCast();
}

void ShadowLayer::onImGuiRender() {
    ImGui::Begin("Shadow map");
    {
        ImVec2 wsize = ImGui::GetWindowSize();
        ImGui::Image((void *)(intptr_t)m_framebuffer->getColorAttachmentId(0),
                     wsize, ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
}

}  // namespace te
