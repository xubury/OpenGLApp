#include "Graphic/Renderer.hpp"
#include "Graphic/OpenGL.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace te {

Renderer::SceneData Renderer::s_sceneData;

Renderer::RenderState Renderer::s_state;

void Renderer::init() {
    s_sceneData.cameraUBO = createRef<UniformBuffer>(sizeof(CameraData));
    s_sceneData.lightUBO = createRef<UniformBuffer>(sizeof(ShadowData));
}

void Renderer::beginScene(const Camera &camera,
                          const FrameBuffer *framebuffer) {
    TE_CORE_ASSERT(
        s_state == RenderState::RENDER_NONE,
        "Render state conflict detected! Please check beginScene() and "
        "endScene() call!");
    s_state = RenderState::RENDER_SCENE;
    if (framebuffer != nullptr) {
        framebuffer->bind();
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glViewport(camera.getViewportX(), camera.getViewportY(),
               camera.getViewportWidth(), camera.getViewportHeight());

    s_sceneData.viewportSize = camera.getViewportSize();

    s_sceneData.cameraUBO->setData(
        glm::value_ptr(camera.getProjection() * camera.getView()),
        offsetof(CameraData, projectionView),
        sizeof(CameraData::projectionView));
    s_sceneData.cameraUBO->setData(glm::value_ptr(camera.getPosition()),
                                   offsetof(CameraData, viewPos),
                                   sizeof(CameraData::viewPos));
}

void Renderer::endScene() { s_state = RenderState::RENDER_NONE; }

void Renderer::beginShadowCast(const glm::mat4 &lightSpaceMatrix,
                               const glm::vec3 &lightDir,
                               const LightBase &light,
                               const Ref<FrameBuffer> &framebuffer) {
    TE_CORE_ASSERT(
        s_state == RenderState::RENDER_NONE,
        "Render state conflict detected! Please check beginShadowCast() and "
        "endShadowCast() call!");
    s_state = RenderState::RENDER_SHADOW;
    framebuffer->bind();
    clear(0.f, 0.f, 0.f);
    glCullFace(GL_FRONT);
    glViewport(0, 0, framebuffer->getSpecification().width,
               framebuffer->getSpecification().height);
    s_sceneData.lightUBO->setData(glm::value_ptr(lightSpaceMatrix),
                                  offsetof(ShadowData, lightSpaceMatrix),
                                  sizeof(ShadowData::lightSpaceMatrix));

    s_sceneData.lightUBO->setData(glm::value_ptr(lightDir),
                                  offsetof(ShadowData, direction),
                                  sizeof(ShadowData::direction));

    s_sceneData.lightUBO->setData(glm::value_ptr(light.ambient),
                                  offsetof(ShadowData, ambient),
                                  sizeof(ShadowData::ambient));

    s_sceneData.lightUBO->setData(glm::value_ptr(light.specular),
                                  offsetof(ShadowData, specular),
                                  sizeof(ShadowData::specular));

    s_sceneData.lightUBO->setData(glm::value_ptr(light.diffuse),
                                  offsetof(ShadowData, diffuse),
                                  sizeof(ShadowData::diffuse));
    s_sceneData.shadowMap = framebuffer->getDepthAttachmentId();
}

void Renderer::endShadowCast() {
    glCullFace(GL_BACK);
    s_state = RenderState::RENDER_NONE;
}

void Renderer::beginGBuffer(const Camera &camera,
                            const FrameBuffer *framebuffer, uint32_t gPosition,
                            uint32_t gNormal, uint32_t gAlbedoSpec,
                            uint32_t gAmbient) {
    TE_CORE_ASSERT(
        s_state == RenderState::RENDER_NONE,
        "Render state conflict detected! Please check beginGBuffer() and "
        "endGBuffer() call!");
    glDisable(GL_BLEND);
    s_state = RenderState::RENDER_GBUFFER;
    if (framebuffer != nullptr) {
        framebuffer->bind();
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glViewport(0, 0, framebuffer->getSpecification().width,
               framebuffer->getSpecification().height);

    s_sceneData.cameraUBO->setData(
        glm::value_ptr(camera.getProjection() * camera.getView()),
        offsetof(CameraData, projectionView),
        sizeof(CameraData::projectionView));
    s_sceneData.cameraUBO->setData(glm::value_ptr(camera.getPosition()),
                                   offsetof(CameraData, viewPos),
                                   sizeof(CameraData::viewPos));
    s_sceneData.viewportSize = camera.getViewportSize();
    s_sceneData.gPosition = gPosition;
    s_sceneData.gNormal = gNormal;
    s_sceneData.gAlbedoSpec = gAlbedoSpec;
    s_sceneData.gAmbient = gAmbient;
}

void Renderer::endGBuffer() {
    s_state = RenderState::RENDER_NONE;
    glEnable(GL_BLEND);
}

void Renderer::submit(const Shader &shader, const VertexArray &vertexArray,
                      GLenum type, bool indexed, const glm::mat4 &transform,
                      const Material *material) {
    shader.bind();
    prepareTextures(shader, material);
    if (s_state == RenderState::RENDER_SCENE) {
        shader.setUniformBlock("Camera",
                               s_sceneData.cameraUBO->getBindingPoint());
    }
    shader.setUniformBlock("Light", s_sceneData.lightUBO->getBindingPoint());
    shader.setMat4("uModel", transform);
    vertexArray.bind();
    if (indexed) {
        uint32_t count = vertexArray.getIndexBuffer()->getCount();
        vertexArray.getIndexBuffer()->bind();
        glDrawElements(type, count, GL_UNSIGNED_INT, nullptr);
    } else {
        for (const auto &buffer : vertexArray.getVertexBuffers()) {
            std::size_t cnt =
                buffer->getSize() / buffer->getLayout().getStride();
            glDrawArrays(type, 0, cnt);
        }
    }
}

void Renderer::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::prepareTextures(const Shader &shader, const Material *material) {
    std::size_t textureIndex = 0;
    uint32_t ambient = 0;
    uint32_t diffuse = 0;
    uint32_t specular = 0;

    if (s_state != RenderState::RENDER_SHADOW && material != nullptr) {
        for (const auto &[texture, type] : material->getList()) {
            std::string name;
            if (type == Material::TEXTURE_AMBIENT) {
                name = "uMaterial.ambient" + std::to_string(ambient++);
            } else if (type == Material::TEXTURE_DIFFUSE) {
                name = "uMaterial.diffuse" + std::to_string(diffuse++);
            } else if (type == Material::TEXTURE_SPECULAR) {
                name = "uMaterial.specular" + std::to_string(specular++);
            } else {
                TE_CORE_WARN("Invalid Texture");
            }
            glActiveTexture(GL_TEXTURE0 + textureIndex);
            texture->bind();
            // set the GL_TEXTUREX correspondence
            shader.setInt(name, textureIndex);
            ++textureIndex;
        }
    }

    if (s_state == RenderState::RENDER_SCENE) {
        glActiveTexture(GL_TEXTURE0 + textureIndex);
        glBindTexture(GL_TEXTURE_2D, s_sceneData.shadowMap);
        shader.setInt("uShadowMap", textureIndex++);

        glActiveTexture(GL_TEXTURE0 + textureIndex);
        glBindTexture(GL_TEXTURE_2D, s_sceneData.gPosition);
        shader.setInt("uGBufferPosition", textureIndex++);
        glActiveTexture(GL_TEXTURE0 + textureIndex);
        glBindTexture(GL_TEXTURE_2D, s_sceneData.gNormal);
        shader.setInt("uGBufferNormal", textureIndex++);
        glActiveTexture(GL_TEXTURE0 + textureIndex);
        glBindTexture(GL_TEXTURE_2D, s_sceneData.gAlbedoSpec);
        shader.setInt("uGBufferAlbedoSpec", textureIndex++);
        glActiveTexture(GL_TEXTURE0 + textureIndex);
        glBindTexture(GL_TEXTURE_2D, s_sceneData.gAmbient);
        shader.setInt("uGBufferAmbient", textureIndex++);
    }
}

}  // namespace te
