#include <Graphic/RenderTarget.hpp>

#include <glad/glad.h>
#include <iostream>

#include <Graphic/Shader.hpp>
#include <Graphic/TextureArray.hpp>
#include <Graphic/VertexBuffer.hpp>
#include <Graphic/ElementBuffer.hpp>

RenderTarget::RenderTarget()
    : m_camera(new Camera(Camera::Default)),
      m_shader(nullptr),
      m_textures(nullptr) {}

Camera &RenderTarget::getCamera() { return *m_camera.get(); }

const Camera &RenderTarget::getCamera() const { return *m_camera.get(); }

bool RenderTarget::processEvent(Event &event) const {
    return m_camera->processEvent(event);
}

void RenderTarget::processEvents() const { m_camera->processEvents(); }

void RenderTarget::draw(const Drawable &drawable, const RenderStates &states) {
    drawable.draw(*this, states);
}

void RenderTarget::draw(const VertexBuffer &buffer,
                        const RenderStates &states) {
    glViewport(m_camera->getX(), m_camera->getY(), m_camera->getWidth(),
               m_camera->getHeight());

    applyShader(states.shader);
    applyTransform(states.transform);
    applyTexture(states.textures);

    m_shader->bind();
    buffer.bind();
    m_shader->setupAttribute();

    buffer.drawPrimitive();
}

void RenderTarget::draw(const ElementBuffer &element,
                        const RenderStates &states) {
    glViewport(m_camera->getX(), m_camera->getY(), m_camera->getWidth(),
               m_camera->getHeight());

    applyShader(states.shader);
    applyTransform(states.transform);
    applyTexture(states.textures);

    m_shader->bind();
    element.bind();
    m_shader->setupAttribute();

    element.drawPrimitive();
}

void RenderTarget::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTarget::applyShader(const Shader *shader) {
    assert(shader != nullptr);
    m_shader = shader;
    m_shader->use();
}

void RenderTarget::applyTransform(const glm::mat4 &transform) const {
    m_shader->setMat4("projection", m_camera->getProjection());
    m_shader->setMat4("view", m_camera->getView());
    m_shader->setMat4("model", transform);
    m_shader->setVec3("viewPos", m_camera->getPosition());
}

void RenderTarget::applyTexture(const TextureArray *textures) {
    if (textures == nullptr || textures == m_textures) return;
    if (m_textures != nullptr) {
        // clean up old textures
        std::size_t size = m_textures->size();
        for (std::size_t i = 0; i < size; ++i) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    m_textures = textures;
    m_shader->setFloat("material.shininess", 64);
    uint32_t diffuse = 0;
    uint32_t specular = 0;
    std::size_t size = m_textures->size();
    for (std::size_t i = 0; i < size; ++i) {
        std::string name;
        if (m_textures->at(i).getType() == Texture::DIFFUSE) {
            name = "material.diffuse" + std::to_string(diffuse++);
        } else if (m_textures->at(i).getType() == Texture::SPECULAR) {
            name = "material.specular" + std::to_string(specular++);
        } else {
            std::cout << "Invalid Texture." << std::endl;
        }
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textures->at(i).id());
        // set the GL_TEXTUREX correspondence
        m_shader->setInt(name, i);
    }
}
