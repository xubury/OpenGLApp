#ifndef VERTEX_ELEMENT_HPP
#define VERTEX_ELEMENT_HPP

#include <glad/glad.h>
#include <stdint.h>
#include <vector>
#include <iostream>

#include <Graphic/Vertex.hpp>
#include <Graphic/Drawable.hpp>

class ElementBuffer : public Drawable {
   public:
    ElementBuffer(int type);

    ~ElementBuffer();

    ElementBuffer(const ElementBuffer &) = delete;

    ElementBuffer &operator=(const ElementBuffer &) = delete;

    std::size_t size() const;

    bool empty() const;

    template <typename T>
    bool create(const T vertices, std::size_t vertexCnt,
                const uint32_t *indices, std::size_t indexCnt);

    template <typename T>
    void update(const T vertices, std::size_t vertexCnt,
                const uint32_t *indices, std::size_t indexCnt);

    template <typename T>
    void update(const T vertices, std::size_t vertexCnt);

    void draw(RenderTarget &target, RenderStates states) const override;

    void bind() const;

    void drawPrimitive() const;

   private:
    uint32_t m_VBO;
    uint32_t m_EBO;
    std::size_t m_size;
    int m_drawType;
};

template <typename T>
bool ElementBuffer::create(const T vertices, std::size_t vertexCnt,
                           const uint32_t *indices, std::size_t indexCnt) {
    glGenBuffers(1, &m_EBO);
    glGenBuffers(1, &m_VBO);
    if (!m_VBO || !m_EBO) {
        std::cerr << "Could not create vertex buffer array." << std::endl;
        return false;
    }
    update(vertices, vertexCnt, indices, indexCnt);
    return true;
}
template <typename T>
void ElementBuffer::update(const T vertices, std::size_t vertexCnt,
                           const uint32_t *indices, std::size_t indexCnt) {
    update(vertices, vertexCnt);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indexCnt, indices,
                 GL_STATIC_DRAW);
    m_size = indexCnt;
}

template <typename T>
void ElementBuffer::update(const T vertices, std::size_t vertexCnt) {
    static_assert(std::is_pointer<T>::value, "Expected a pointer");
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertexCnt * sizeof(typename std::remove_pointer<T>::type),
                 vertices, GL_STATIC_DRAW);
}

#endif
