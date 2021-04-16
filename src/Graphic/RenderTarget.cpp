#include <Graphic/RenderTarget.hpp>

#include <glad/glad.h>
#include <iostream>

#include <Graphic/Shader.hpp>
#include <Graphic/TextureArray.hpp>
#include <Graphic/VertexBuffer.hpp>
#include <Graphic/Vertex.hpp>

RenderTarget::RenderTarget() : m_VAO(0), m_camera(new Camera(Camera::Default)) {
    glGenVertexArrays(1, &m_VAO);
    if (!m_VAO) {
        std::cerr << "Could not create vertex buffer." << std::endl;
    }
}

RenderTarget::~RenderTarget() { glDeleteVertexArrays(1, &m_VAO); }

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
    applyTexture(states.textures);

    glDrawArrays(GL_TRIANGLES, 0, buffer.size());

    glBindVertexArray(0);
    VertexBuffer::bind(nullptr);
}

void RenderTarget::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTarget::applyShader(const Shader *shader) { m_shader = shader; }

void RenderTarget::applyTransform(const glm::mat4 &transform) const {
    if (m_shader == nullptr) return;
    m_shader->use();
    m_shader->setMat4("projection", m_camera->getProjection());
    m_shader->setMat4("view", m_camera->getView());
    m_shader->setMat4("model", transform);
}

void RenderTarget::applyTexture(const TextureArray *textures) const {
    if (textures == nullptr) return;
    // set the GL_TEXTUREX correspondence
    m_shader->use();
    m_shader->setFloat("material.shininess", 64);
    uint32_t diffuse = 0;
    uint32_t specular = 0;
    std::size_t size = textures->size();
    for (std::size_t i = 0; i < size; ++i) {
        std::string name;
        if (textures->at(i).getType() == Texture::DIFFUSE) {
            name = "material.diffuse" + std::to_string(diffuse++);
        } else if (textures->at(i).getType() == Texture::SPECULAR) {
            name = "material.specular" + std::to_string(specular++);
        } else {
            std::cout << "Invalid Texture." << std::endl;
        }
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures->at(i).id());
        m_shader->setInt(name, i);
    }
}
