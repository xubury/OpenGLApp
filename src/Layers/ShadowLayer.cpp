#include "Layers/ShadowLayer.hpp"
#include "Core/Application.hpp"
#include "Graphic/Renderer.hpp"
#include "Component/Light.hpp"

namespace te {

const uint32_t SHADOW_MAP_WIDTH = 4096;
const uint32_t SHADOW_MAP_HEIGHT = 4096;

ShadowLayer::ShadowLayer() : Layer("Shadow layer") {
    FrameBufferSpecification spec;
    spec.width = SHADOW_MAP_WIDTH;
    spec.height = SHADOW_MAP_HEIGHT;
    spec.attachmentsSpecs = {{FramebufferTextureFormat::DEPTH32}};
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
        void main() {
            gl_Position = uLightSpaceMatrix * uModel * vec4(aPos, 1.0);
        }

    )";
    const char *shadowFragment =
        "#version 330 core\n"
        "void main() {\n"
        "}";
    m_shader = createRef<Shader>();
    m_shader->compile(shadowVertex, shadowFragment);
}

void ShadowLayer::onRender() {
    Renderer::beginShadowCast(m_framebuffer);
    Ref<SceneManager<EntityBase>> scene =
        Application::instance().getActiveScene();
    Light::Handle light;
    auto view = scene->entities.getByComponents(light);
    auto end = view.end();
    for (auto cur = view.begin(); cur != end; ++cur) {
        if (light->castShadow) {
            // TODO: mutitple shadow caster is not supproted
            Renderer::setShadowCaster(light.get());
            break;
        }
    }
    std::size_t size = scene->entities.size();
    for (std::size_t i = 0; i < size; ++i) {
        scene->entities.get(i)->draw(m_shader);
    }
    Renderer::endShadowCast();
}

}  // namespace te
