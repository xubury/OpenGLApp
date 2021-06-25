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
                             {FramebufferTextureFormat::DEPTH32}};
    m_gBuffer = createScope<FrameBuffer>(spec, true);

    m_shader = createScope<Shader>();
    std::string vertexCode;
    fileToString("shader/vertex.glsl", vertexCode);
    const char *fragmenCode = R"(
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
        float shininess;
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
    m_shader->compile(vertexCode.c_str(), fragmenCode);
}

void GBufferLayer::onRender() {
    Renderer::beginGBuffer(*Application::instance().getMainCamera(),
                           m_gBuffer.get(), m_gBuffer->getColorAttachmentId(0),
                           m_gBuffer->getColorAttachmentId(1),
                           m_gBuffer->getColorAttachmentId(2));
    Renderer::clear(0.f, 0.f, 0.f, 1.f);
    Ref<SceneManager<EntityBase>> scene =
        Application::instance().getActiveScene();
    std::size_t size = scene->entities.size();
    for (std::size_t i = 0; i < size; ++i) {
        scene->entities.get(i)->draw(*m_shader);
    }
    Renderer::endGBuffer();
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

}  // namespace te
