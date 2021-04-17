#include <glad/glad.h>
#include <iostream>

#include <Graphic/Vertex.hpp>
#include <Graphic/VertexElement.hpp>
#include <Graphic/RenderTarget.hpp>

VertexElement::VertexElement() : m_VBO(0), m_EBO(0) {}

VertexElement::~VertexElement() {
    glDeleteBuffers(1, &m_EBO);
    glDeleteBuffers(1, &m_VBO);
}

std::size_t VertexElement::size() const { return m_size; }

bool VertexElement::empty() const { return m_size == 0; }

bool VertexElement::create(const Vertex *vertices, std::size_t vertexSize,
                           uint32_t *indices, std::size_t indexSize) {
    glGenBuffers(1, &m_EBO);
    glGenBuffers(1, &m_VBO);
    if (!m_VBO || !m_EBO) {
        std::cerr << "Could not create vertex buffer array." << std::endl;
        return false;
    }
    update(vertices, vertexSize, indices, indexSize);
    return true;
}

void VertexElement::update(const Vertex *vertices, std::size_t vertexSize,
                           uint32_t *indices, std::size_t indexSize) {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexSize, vertices,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indexSize, indices,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    m_size = indexSize;
}

void VertexElement::draw(RenderTarget &target, RenderStates states) const {
    target.draw(*this, states);
}

void VertexElement::bind(const VertexElement *vertexElement) {
    if (vertexElement != nullptr) {
        glBindBuffer(GL_ARRAY_BUFFER, vertexElement->m_VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexElement->m_EBO);
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
