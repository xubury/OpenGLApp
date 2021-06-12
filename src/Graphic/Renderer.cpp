#include "Graphic/Renderer.hpp"
#include "Core/Application.hpp"
#include "Graphic/OpenGL.hpp"

namespace te {

Renderer::SceneData Renderer::s_sceneData;

void Renderer::init() {}

void Renderer::beginScene(const Ref<Camera> &camera,
                          const Ref<FrameBuffer> &framebuffer) {
    if (framebuffer != nullptr) {
        framebuffer->bind();
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glViewport(camera->getViewportX(), camera->getViewportY(),
               camera->getViewportWidth(), camera->getViewportHeight());
    s_sceneData.projection = camera->getProjection();
    s_sceneData.view = camera->getView();
}

void Renderer::beginShadowCast(const Ref<FrameBuffer> &framebuffer) {
    TE_CORE_ASSERT(s_sceneData.shadowCaster != nullptr, "No light specify!");
    framebuffer->bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    glViewport(0, 0, framebuffer->getSpecification().width,
               framebuffer->getSpecification().height);
    s_sceneData.shadowCaster->shadowMap = framebuffer->getDepthAttachmentId();
}

void Renderer::endShadowCast() { glCullFace(GL_BACK); }

void Renderer::setShadowCaster(LightBase *light) {
    s_sceneData.shadowCaster = light;
}

void Renderer::submit(const Ref<Shader> &shader,
                      const Ref<VertexArray> &vertexArray, GLenum type,
                      bool indexed, const glm::mat4 &transform) {
    shader->bind();
    if (s_sceneData.shadowCaster != nullptr) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, s_sceneData.shadowCaster->shadowMap);
        shader->setInt("uDepthMap", 0);
        shader->setMat4("uLightSpaceMatrix",
                        s_sceneData.shadowCaster->getLightSpaceMatrix());
        shader->setVec3("uDirLight.direction",
                        s_sceneData.shadowCaster->getDirection());
        shader->setVec3("uDirLight.ambient", s_sceneData.shadowCaster->amibent);
        shader->setVec3("uDirLight.diffuse", s_sceneData.shadowCaster->diffuse);
        shader->setVec3("uDirLight.specular",
                        s_sceneData.shadowCaster->specular);
    }
    shader->setMat4("uProjection", s_sceneData.projection);
    shader->setMat4("uView", s_sceneData.view);
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
