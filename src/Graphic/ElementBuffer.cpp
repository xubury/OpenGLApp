#include <glad/glad.h>
#include <iostream>

#include <Graphic/ElementBuffer.hpp>
#include <Graphic/RenderTarget.hpp>

ElementBuffer::ElementBuffer()
    : m_VBO(0),
      m_EBO(0),
      m_VAO(0),
      m_size(0),
      m_mode(GL_STATIC_DRAW),
      m_primitiveType(GL_TRIANGLES),
      m_attrFunction(nullptr) {}

ElementBuffer::~ElementBuffer() {
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteVertexArrays(1, &m_VAO);
}

ElementBuffer::ElementBuffer(const ElementBuffer &other)
    : m_VBO(0),
      m_EBO(0),
      m_VAO(0),
      m_size(other.m_size),
      m_mode(other.m_mode),
      m_primitiveType(other.m_primitiveType),
      m_attrFunction(other.m_attrFunction) {
    initialize();

    int size;
    glBindBuffer(GL_COPY_READ_BUFFER, other.m_VBO);
    glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);
    glBindBuffer(GL_COPY_WRITE_BUFFER, m_VBO);
    glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, other.m_mode);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);

    glBindBuffer(GL_COPY_READ_BUFFER, other.m_EBO);
    glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);
    glBindBuffer(GL_COPY_WRITE_BUFFER, m_EBO);
    glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, other.m_mode);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_attrFunction();
}

ElementBuffer &ElementBuffer::operator=(ElementBuffer other) {
    std::swap(m_VBO, other.m_VBO);
    std::swap(m_EBO, other.m_EBO);
    std::swap(m_VAO, other.m_VAO);
    std::swap(m_size, other.m_size);
    std::swap(m_mode, other.m_mode);
    std::swap(m_primitiveType, other.m_primitiveType);
    std::swap(m_attrFunction, other.m_attrFunction);
    return *this;
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
    glDrawElements(m_primitiveType, m_size, GL_UNSIGNED_INT, 0);
}
