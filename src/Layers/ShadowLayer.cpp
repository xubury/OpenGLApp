#include "Layers/ShadowLayer.hpp"
#include "Core/Application.hpp"
#include "Graphic/Renderer.hpp"

namespace te {

const uint32_t SHADOW_MAP_WIDTH = 4096;
const uint32_t SHADOW_MAP_HEIGHT = 4096;

ShadowLayer::ShadowLayer() : Layer("Shadow layer") {
    FrameBufferSpecification spec;
    spec.width = SHADOW_MAP_WIDTH;
    spec.height = SHADOW_MAP_HEIGHT;
    spec.attachmentsSpecs = {{FramebufferTextureFormat::DEPTH32}};
    m_framebuffer = createScope<FrameBuffer>(spec, false);

    const char *shadowVertex =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 uLightSpaceMatrix;\n"
        "uniform mat4 uModel;\n"
        "void main() {\n"
        "    gl_Position = uLightSpaceMatrix * uModel * vec4(aPos, 1.0);\n"
        "}";
    const char *shadowFragment =
        "#version 330 core\n"
        "void main() {\n"
        "}";
    m_shader = createRef<Shader>();
    m_shader->compile(shadowVertex, shadowFragment);
}

void ShadowLayer::onRender() {
    m_framebuffer->bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    glViewport(0, 0, m_framebuffer->getSpecification().width,
               m_framebuffer->getSpecification().height);

    m_shader->bind();
    LightBase *light = Renderer::getLightSource();
    if (light != nullptr) {
        m_shader->setMat4("uLightSpaceMatrix", light->getLightSpaceMatrix());
        light->shadowMap = m_framebuffer->getDepthAttachmentId();
    }
    Ref<SceneManager<EntityBase>> scene =
        Application::instance().getActiveScene();
    std::size_t size = scene->entities.size();
    for (std::size_t i = 0; i < size; ++i) {
        scene->entities.get(i)->draw(m_shader);
    }

    m_framebuffer->unbind();
    glCullFace(GL_BACK);
}

}  // namespace te
