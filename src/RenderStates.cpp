#include "RenderStates.hpp"

#include <glad/glad.h>

#include "Shader.hpp"
#include "Texture.hpp"

RenderStates::RenderStates() {}

void RenderStates::setShader(const Shader &shader) { m_shader = &shader; }

void RenderStates::setTransform(const glm::mat4 &transform) {
    m_transform = transform;
}

void RenderStates::setTexture(const Texture &texture) { m_texture = &texture; }

void RenderStates::setupShader() const { m_shader->use(); }

void RenderStates::setupTranform() const {
    m_shader->setMat4("model", m_transform);
}

void RenderStates::setupTexture() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->id());
}
