#include "Graphic/Renderer.hpp"
#include "Core/Application.hpp"
#include "Graphic/OpenGL.hpp"

namespace te {

Renderer::SceneData Renderer::s_sceneData;

void Renderer::init() {
    s_sceneData.projectionViewUBO =
        createRef<UniformBuffer>(2 * sizeof(glm::mat4));
    s_sceneData.lightUBO =
        createRef<UniformBuffer>(sizeof(glm::mat4) + 4 * sizeof(glm::vec3));
}

void Renderer::beginScene(const Ref<Camera> &camera,
                          const Ref<FrameBuffer> &framebuffer) {
    if (framebuffer != nullptr) {
        framebuffer->bind();
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glViewport(camera->getViewportX(), camera->getViewportY(),
               camera->getViewportWidth(), camera->getViewportHeight());

    s_sceneData.projectionViewUBO->clearData();
    s_sceneData.projectionViewUBO->setData(
        glm::value_ptr(camera->getProjection()), sizeof(glm::mat4));
    s_sceneData.projectionViewUBO->setData(glm::value_ptr(camera->getView()),
                                           sizeof(glm::mat4));
}

void Renderer::beginShadowCast(const Ref<FrameBuffer> &framebuffer) {
    framebuffer->bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    glViewport(0, 0, framebuffer->getSpecification().width,
               framebuffer->getSpecification().height);
    s_sceneData.shadowMap = framebuffer->getDepthAttachmentId();
}

void Renderer::endShadowCast() { glCullFace(GL_BACK); }

void Renderer::setShadowCaster(LightBase *light) {
    s_sceneData.lightUBO->clearData();
    s_sceneData.lightUBO->setData(glm::value_ptr(light->getLightSpaceMatrix()),
                                  sizeof(glm::mat4));
    s_sceneData.lightUBO->setData(glm::value_ptr(light->getDirection()),
                                  sizeof(glm::vec3));
    s_sceneData.lightUBO->setData(glm::value_ptr(light->amibent),
                                  sizeof(glm::vec3));
    s_sceneData.lightUBO->setData(glm::value_ptr(light->specular),
                                  sizeof(glm::vec3));
    s_sceneData.lightUBO->setData(glm::value_ptr(light->diffuse),
                                  sizeof(glm::vec3));
}

void Renderer::submit(const Ref<Shader> &shader,
                      const Ref<VertexArray> &vertexArray, GLenum type,
                      bool indexed, const glm::mat4 &transform) {
    shader->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s_sceneData.shadowMap);
    shader->setInt("uShadowMap", 0);
    shader->setUniformBlock("Light", s_sceneData.lightUBO->getBindingPoint());
    shader->setUniformBlock("ProjectionView",
                            s_sceneData.projectionViewUBO->getBindingPoint());
    shader->setMat4("uModel", transform);
    vertexArray->bind();
    if (indexed) {
        uint32_t count = vertexArray->getIndexBuffer()->getCount();
        vertexArray->getIndexBuffer()->bind();
        glDrawElements(type, count, GL_UNSIGNED_INT, nullptr);
    } else {
        for (const auto &buffer : vertexArray->getVertexBuffers()) {
            std::size_t cnt =
                buffer->getSize() / buffer->getLayout().getStride();
            glDrawArrays(type, 0, cnt);
        }
    }
}

void Renderer::endScene() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void Renderer::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

}  // namespace te
