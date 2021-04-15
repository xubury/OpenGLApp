#include "Graphic/RenderTarget.hpp"

#include <glad/glad.h>
#include <iostream>

#include "Graphic/Shader.hpp"
#include "Graphic/Texture.hpp"
#include "Graphic/VertexBuffer.hpp"
#include "Graphic/Vertex.hpp"

RenderTarget::RenderTarget() : m_VAO(0), m_camera(new Camera(Camera::Default)) {
    glGenVertexArrays(1, &m_VAO);
    if (!m_VAO) {
        std::cerr << "Could not create vertex buffer." << std::endl;
    }
}

RenderTarget::~RenderTarget() { glDeleteVertexArrays(1, &m_VAO); }

Camera *RenderTarget::getCamera() { return m_camera.get(); }

void RenderTarget::setCamera(std::unique_ptr<Camera> camera) {
    m_camera = std::move(camera);
}

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
    VertexBuffer::bind(&buffer);
    glBindVertexArray(m_VAO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    // texture coord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)(offsetof(Vertex, texCoords)));
    glEnableVertexAttribArray(2);

    // normal attribute
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(3);

    applyShader(states.shader);
    applyTransform(states.transform);
    applyTexture(states.texture);

    glDrawArrays(GL_TRIANGLES, 0, buffer.size());
    VertexBuffer::bind(nullptr);
}

void RenderTarget::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTarget::applyShader(const Shader *shader) { m_shader = shader; }

void RenderTarget::applyTransform(const glm::mat4 &transform) const {
    m_shader->use();
    m_shader->setMat4("projection", m_camera->getProjection());
    m_shader->setMat4("view", m_camera->getView());
    m_shader->setMat4("model", transform);
}

void RenderTarget::applyTexture(const Texture *texture) const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id(Texture::DIFFUSE));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture->id(Texture::SPECULAR));
}
