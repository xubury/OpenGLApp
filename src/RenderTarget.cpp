#include "RenderTarget.hpp"

#include <glad/glad.h>
#include <iostream>

#include "RenderStates.hpp"
#include "Drawable.hpp"
#include "VertexBuffer.hpp"
#include "Vertex.hpp"

RenderTarget::RenderTarget() : m_view(1.0f), m_VAO(0) {}

bool RenderTarget::create() {
    if (!m_VAO) glGenVertexArrays(1, &m_VAO);
    if (!m_VAO) {
        std::cerr << "Could not create vertex buffer." << std::endl;
        return false;
    }
    return true;
}

void RenderTarget::setView(const glm::mat4 &view) { m_view = view; }

void RenderTarget::draw(const Drawable &drawable, const RenderStates &states) {
    drawable.draw(*this, states);
}

void RenderTarget::draw(const VertexBuffer &buffer,
                        const RenderStates &states) {
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

    states.setupShader();
    states.setupTranform();
    states.setupTexture();
    glDrawArrays(GL_TRIANGLES, 0, buffer.size());
    VertexBuffer::bind(nullptr);
}

void RenderTarget::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
