#include "RenderTarget.hpp"

#include <glad/glad.h>

#include "Drawable.hpp"
#include "VertexBuffer.hpp"
#include "Vertex.hpp"

RenderTarget::RenderTarget() : m_view(1.0f), m_VAO(0) {}

void RenderTarget::setView(const glm::mat4 &view) { m_view = view; }

void RenderTarget::draw(const Drawable &drawable) { drawable.draw(*this); }

void RenderTarget::draw(const VertexBuffer &buffer) {
    VertexBuffer::bind(&buffer);
    if (m_VAO == 0) {
        glGenVertexArrays(1, &m_VAO);
    }

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

    glDrawArrays(GL_TRIANGLES, 0, buffer.size());

    VertexBuffer::bind(nullptr);
}
