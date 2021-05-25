#include <glad/glad.h>
#include <iostream>

#include <Graphic/ElementBuffer.hpp>
#include <Graphic/RenderTarget.hpp>

ElementBuffer::ElementBuffer()
    : m_VBO(0), m_EBO(0), m_VAO(0), m_size(0), m_drawType(GL_TRIANGLES) {}

ElementBuffer::~ElementBuffer() {
    glDeleteBuffers(1, &m_EBO);
    glDeleteBuffers(1, &m_VBO);
}

bool ElementBuffer::initialize() {
    if (isInit()) return true;
    glGenBuffers(1, &m_VBO);
    if (!m_VBO) {
        std::cerr << "Could not create vertex buffer." << std::endl;
        return false;
    }
    glGenBuffers(1, &m_EBO);
    if (!m_EBO) {
        std::cerr << "Could not create element buffer object." << std::endl;
        return false;
    }
    glGenVertexArrays(1, &m_VAO);
    if (!m_VAO) {
        std::cerr << "Could not create vertex array object." << std::endl;
        return false;
    }
    return true;
}

std::size_t ElementBuffer::size() const { return m_size; }

bool ElementBuffer::empty() const { return m_size == 0; }

bool ElementBuffer::isInit() const { return m_VAO != 0; }

void ElementBuffer::draw(RenderTarget &target, RenderStates states) const {
    target.draw(*this, states);
}

void ElementBuffer::drawPrimitive() const {
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glDrawElements(m_drawType, m_size, GL_UNSIGNED_INT, 0);
}
