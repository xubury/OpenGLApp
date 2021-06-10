#include "Graphic/Renderer.hpp"
#include "Graphic/OpenGL.hpp"

namespace te {

Renderer::SceneData Renderer::s_sceneData;

void Renderer::init() {}

void Renderer::beginScene(const CameraBase &camera) {
    glViewport(camera.getViewportX(), camera.getViewportY(),
               camera.getViewportWidth(), camera.getViewportHeight());
    s_sceneData.projection = camera.getProjection();
    s_sceneData.view = camera.getView();
}

void Renderer::submit(const Ref<Shader> &shader,
                      const Ref<VertexArray> &vertexArray,
                      const glm::mat4 &transform) {
    shader->bind();
    shader->setMat4("uProjection", s_sceneData.projection);
    shader->setMat4("uView", s_sceneData.view);
    shader->setMat4("uModel", transform);
    vertexArray->bind();
    if (vertexArray->getIndexBuffer() == nullptr) {
        for (const auto &buffer : vertexArray->getVertexBuffers()) {
            std::size_t cnt =
                buffer->getSize() / buffer->getLayout().getStride();
            glDrawArrays(vertexArray->getType(), 0, cnt);
        }
    } else {
        uint32_t count = vertexArray->getIndexBuffer()->getCount();
        vertexArray->getIndexBuffer()->bind();
        glDrawElements(vertexArray->getType(), count, GL_UNSIGNED_INT, nullptr);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::endScene() {}

void Renderer::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

}  // namespace te
