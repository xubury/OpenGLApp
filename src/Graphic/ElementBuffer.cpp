#include <glad/glad.h>
#include <iostream>

#include <Graphic/ElementBuffer.hpp>
#include <Graphic/RenderTarget.hpp>

ElementBuffer::ElementBuffer() : m_VBO(0), m_EBO(0) {}

ElementBuffer::~ElementBuffer() {
    glDeleteBuffers(1, &m_EBO);
    glDeleteBuffers(1, &m_VBO);
}

std::size_t ElementBuffer::size() const { return m_size; }

bool ElementBuffer::empty() const { return m_size == 0; }

void ElementBuffer::draw(RenderTarget &target, RenderStates states) const {
    target.draw(*this, states);
}

void ElementBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
}
