#include "Layers/GBufferLayer.hpp"
#include "Graphic/Renderer.hpp"
#include "Apps/Application.hpp"
#include "Utils/File.hpp"

namespace te {

GBufferLayer::GBufferLayer(int width, int height, int samples)
    : Layer("GBuffer Layer") {
    FrameBufferSpecification spec;
    spec.width = width;
    spec.height = height;
    spec.samples = samples;
    spec.attachmentsSpecs = {{FramebufferTextureFormat::RGBA16F},
                             {FramebufferTextureFormat::RGBA16F},
                             {FramebufferTextureFormat::RGBA16F},
                             {FramebufferTextureFormat::RGB16F},
                             {FramebufferTextureFormat::DEPTH24}};
    m_multiSampledGBuffer = createScope<FrameBuffer>(spec, true);
    spec.attachmentsSpecs = {{FramebufferTextureFormat::RGBA16F},
                             {FramebufferTextureFormat::RGBA16F},
                             {FramebufferTextureFormat::RGBA16F},
                             {FramebufferTextureFormat::RGB16F}};
    spec.samples = 0;
    m_gBuffer = createScope<FrameBuffer>(spec);

    m_gBufferShader = createScope<Shader>();
    std::string vertexCode;
    fileToString("shader/vertex.glsl", vertexCode);
    const char *fragmentCode = R"(
    #version 330 core
    layout (location = 0) out vec4 gPosition;
    layout (location = 1) out vec4 gNormal;
    layout (location = 2) out vec4 gAlbedoSpec;
    layout (location = 3) out vec3 gAmbient;

    struct Material {
        sampler2D ambient0;
    
        sampler2D diffuse0;
        sampler2D diffuse1;
        sampler2D diffuse2;
    
        sampler2D specular0;
        sampler2D specular1;
    };
    uniform Material uMaterial;

    in vec3 fragPos;
    in vec2 texCoord;
    in vec3 normal;
    void main() {    
        // store the fragment position vector in the first gbuffer texture
        gPosition.rgb = fragPos;
        gPosition.a = 1.0f;
        // also store the per-fragment normals into the gbuffer
        gNormal.rgb = normalize(normal);
        gNormal.a = 1.0f;

        // and the diffuse per-fragment color
        gAlbedoSpec.rgb = texture(uMaterial.diffuse0, texCoord).rgb;
        // store specular intensity in gAlbedoSpec's alpha component
        gAlbedoSpec.a = texture(uMaterial.specular0, texCoord).r;

        gAmbient = texture(uMaterial.ambient0, texCoord).rgb;
    })";
    m_gBufferShader->compile(vertexCode.c_str(), fragmentCode);
}

static void copyMultiSampleGbuffer(const FrameBuffer &src,
                                   const FrameBuffer &dst) {
    for (int i = 0; i < 4; ++i) {
        src.copy(dst, GL_COLOR_ATTACHMENT0 + i, GL_COLOR_BUFFER_BIT,
                 GL_NEAREST);
    }
    const FrameBuffer *defaultFrameBuffer =
        Application::instance().getFramebuffer();
    glBindFramebuffer(GL_READ_FRAMEBUFFER, src.getId());
    if (defaultFrameBuffer != nullptr) {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, defaultFrameBuffer->getId());
    } else {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }
    glBlitFramebuffer(
        0, 0, src.getSpecification().width, src.getSpecification().height, 0, 0,
        src.getSpecification().width, src.getSpecification().height,
        GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

void GBufferLayer::onRender() {
    Renderer::beginGBuffer(
        *Application::instance().getMainCamera(), m_multiSampledGBuffer.get(),
        m_gBuffer->getColorAttachmentId(0), m_gBuffer->getColorAttachmentId(1),
        m_gBuffer->getColorAttachmentId(2), m_gBuffer->getColorAttachmentId(3));
    Renderer::clear(0.f, 0.f, 0.f, 1.f);
    Ref<SceneManager<EntityBase>> scene =
        Application::instance().getActiveScene();
    std::size_t size = scene->entities.size();
    for (std::size_t i = 0; i < size; ++i) {
        scene->entities.get(i)->draw(*m_gBufferShader);
    }
    Renderer::endGBuffer();
    copyMultiSampleGbuffer(*m_multiSampledGBuffer, *m_gBuffer);
}

void GBufferLayer::onImGuiRender() {
    ImGui::Begin("Geometry Buffer");
    {
        ImVec2 wsize = ImGui::GetWindowSize();
        ImVec2 cellSize = wsize;
        cellSize.y /= 4;
        for (int i = 0; i < 4; ++i) {
            ImGui::Image((void *)(intptr_t)m_gBuffer->getColorAttachmentId(i),
                         cellSize, ImVec2(0, 1), ImVec2(1, 0));
        }
    }
    ImGui::End();
}

void GBufferLayer::onUpdate(const Time &) {
    const FrameBuffer *defaultFrameBuffer =
        Application::instance().getFramebuffer();

    uint32_t width = 0;
    uint32_t height = 0;
    if (defaultFrameBuffer != nullptr) {
        width = defaultFrameBuffer->getSpecification().width;
        height = defaultFrameBuffer->getSpecification().height;
    } else {
        width = Application::instance().getWindow().width();
        height = Application::instance().getWindow().height();
    }
    if (m_multiSampledGBuffer->getSpecification().width != width ||
        m_multiSampledGBuffer->getSpecification().height != height) {
        m_multiSampledGBuffer->resize(width, height);
        m_gBuffer->resize(width, height);
    }
}

void GBufferLayer::onEventPoll(const Event &event) {
    if (event.type == Event::RESIZED) {
        m_multiSampledGBuffer->resize(event.size.width, event.size.height);
        m_gBuffer->resize(event.size.width, event.size.height);
    }
}

}  // namespace te
