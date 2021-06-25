#include "Layers/GBufferLayer.hpp"
#include "Graphic/Renderer.hpp"
#include "Apps/Application.hpp"
#include "Utils/File.hpp"

namespace te {

GBufferLayer::GBufferLayer(int width, int height) : Layer("GBuffer Layer") {
    FrameBufferSpecification spec;
    spec.width = width;
    spec.height = height;
    spec.attachmentsSpecs = {{FramebufferTextureFormat::RGBA16F},
                             {FramebufferTextureFormat::RGBA16F},
                             {FramebufferTextureFormat::RGBA8},
                             {FramebufferTextureFormat::DEPTH24}};
    m_gBuffer = createScope<FrameBuffer>(spec, true);

    m_gBufferShader = createScope<Shader>();
    std::string vertexCode;
    fileToString("shader/vertex.glsl", vertexCode);
    const char *fragmentCode = R"(
    #version 330 core
    layout (location = 0) out vec4 gPosition;
    layout (location = 1) out vec4 gNormal;
    layout (location = 2) out vec4 gAlbedoSpec;

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
    })";
    m_gBufferShader->compile(vertexCode.c_str(), fragmentCode);

    const char *deferredVertex = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;
    
    out vec2 texCoord;
    
    void main() {
        texCoord = aTexCoord;
        gl_Position = vec4(aPos, 1.0);
    })";
    std::string deferredFragment;
    fileToString("shader/pointLight.glsl", deferredFragment);
    m_deferredShader = createScope<Shader>();
    m_deferredShader->compile(deferredVertex, deferredFragment.c_str());
    m_deferredShader->bind();
    m_deferredShader->setVec3("uPointLight.position",
                              glm::vec3(0.f, 4.0f, 0.f));
    m_deferredShader->setVec3("uPointLight.direction",
                              glm::vec3(0.0f, -1.0f, 0.0f));
    m_deferredShader->setVec3("uPointLight.ambient", glm::vec3(1.0f));
    m_deferredShader->setVec3("uPointLight.diffuse", glm::vec3(1.0f));
    m_deferredShader->setVec3("uPointLight.specular", glm::vec3(1.0f));
    m_deferredShader->setFloat("uPointLight.constant", 1.0f);
    m_deferredShader->setFloat("uPointLight.linear", 0.09f);
    m_deferredShader->setFloat("uPointLight.quadratic", 0.032f);
    m_deferredShader->setFloat("uPointLight.cutOff",
                               glm::cos(glm::radians(12.5f)));
    m_deferredShader->setFloat("uPointLight.outerCutOff",
                               glm::cos(glm::radians(67.5f)));

    float quadVertices[] = {
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
    };
    Ref<VertexBuffer> buffer =
        createRef<VertexBuffer>(quadVertices, sizeof(quadVertices));
    buffer->setLayout({{ShaderDataType::Float3, "aPos"},
                       {ShaderDataType::Float2, "aTexCoord"}});
    m_quad = createScope<VertexArray>();
    m_quad->addVertexBuffer(buffer);
}

void GBufferLayer::onRender() {
    Renderer::beginGBuffer(*Application::instance().getMainCamera(),
                           m_gBuffer.get());
    Renderer::clear(0.f, 0.f, 0.f, 1.f);
    Ref<SceneManager<EntityBase>> scene =
        Application::instance().getActiveScene();
    std::size_t size = scene->entities.size();
    for (std::size_t i = 0; i < size; ++i) {
        scene->entities.get(i)->draw(*m_gBufferShader);
    }
    Renderer::endGBuffer();

    Renderer::beginScene(*Application::instance().getMainCamera(),
                         Application::instance().getFramebuffer());
    Renderer::clear(0.f, 0.f, 0.f, 1.f);
    m_deferredShader->bind();
    int textureIndex = 0;
    glActiveTexture(GL_TEXTURE0 + textureIndex);
    glBindTexture(GL_TEXTURE_2D, m_gBuffer->getColorAttachmentId(0));
    m_deferredShader->setInt("uGBufferPosition", textureIndex++);
    glActiveTexture(GL_TEXTURE0 + textureIndex);
    glBindTexture(GL_TEXTURE_2D, m_gBuffer->getColorAttachmentId(1));
    m_deferredShader->setInt("uGBufferNormal", textureIndex++);
    glActiveTexture(GL_TEXTURE0 + textureIndex);
    glBindTexture(GL_TEXTURE_2D, m_gBuffer->getColorAttachmentId(2));
    m_deferredShader->setInt("uGBufferAlbedoSpec", textureIndex++);

    m_quad->bind();
    Ref<VertexBuffer> buffer = m_quad->getVertexBuffers()[0];
    std::size_t cnt = buffer->getSize() / buffer->getLayout().getStride();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, cnt);

    const FrameBuffer *defaultFrameBuffer =
        Application::instance().getFramebuffer();
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer->getId());
    if (defaultFrameBuffer != nullptr) {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, defaultFrameBuffer->getId());
    } else {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }
    glBlitFramebuffer(0, 0, m_gBuffer->getSpecification().width,
                      m_gBuffer->getSpecification().height, 0, 0,
                      m_gBuffer->getSpecification().width,
                      m_gBuffer->getSpecification().height, GL_DEPTH_BUFFER_BIT,
                      GL_NEAREST);
    Renderer::endScene();
}

void GBufferLayer::onImGuiRender() {
    ImGui::Begin("Position Buffer");
    {
        ImVec2 wsize = ImGui::GetWindowSize();
        ImGui::Image((void *)(intptr_t)m_gBuffer->getColorAttachmentId(0),
                     wsize, ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
    ImGui::Begin("Normal Buffer");
    {
        ImVec2 wsize = ImGui::GetWindowSize();
        ImGui::Image((void *)(intptr_t)m_gBuffer->getColorAttachmentId(1),
                     wsize, ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
    ImGui::Begin("Albedo Buffer");
    {
        ImVec2 wsize = ImGui::GetWindowSize();
        ImGui::Image((void *)(intptr_t)m_gBuffer->getColorAttachmentId(2),
                     wsize, ImVec2(0, 1), ImVec2(1, 0));
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
    if (m_gBuffer->getSpecification().width != width ||
        m_gBuffer->getSpecification().height != height) {
        m_gBuffer->resize(width, height);
    }
}

void GBufferLayer::onEventPoll(const Event &event) {
    if (event.type == Event::RESIZED) {
        m_gBuffer->resize(event.size.width, event.size.height);
    }
}

}  // namespace te
