#ifndef VERTEX_ELEMENT_HPP
#define VERTEX_ELEMENT_HPP

#include <stdint.h>
#include <vector>
#include <iostream>

#include <Graphic/Export.hpp>
#include <Graphic/Vertex.hpp>
#include <Graphic/Drawable.hpp>
#include <Graphic/OpenGL.hpp>

typedef void (*AttrFunc)();

class GRAPHIC_API ElementBuffer : public Drawable {
   public:
    ElementBuffer();

    ~ElementBuffer();

    ElementBuffer(const ElementBuffer &other);

    ElementBuffer &operator=(ElementBuffer other);

    std::size_t size() const;

    bool empty() const;

    bool isInit() const;

    bool initialize();

    template <typename T>
    void update(const T vertices, std::size_t vertexCnt,
                const uint32_t *indices, std::size_t indexCnt, GLenum type,
                GLenum mode);

    template <typename T>
    void update(const T vertices, std::size_t vertexCnt, GLenum type,
                GLenum mode);

    void draw(RenderTarget &target, RenderStates states) const override;

    void drawPrimitive() const;

   private:
    uint32_t m_VBO;
    uint32_t m_EBO;
    uint32_t m_VAO;
    std::size_t m_size;
    GLenum m_mode;
    GLenum m_primitiveType;
    AttrFunc m_attrFunction;
};

template <typename T>
void ElementBuffer::update(const T vertices, std::size_t vertexCnt,
                           const uint32_t *indices, std::size_t indexCnt,
                           GLenum type, GLenum mode) {
    update(vertices, vertexCnt, type, mode);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indexCnt, indices,
                 mode);
    m_size = indexCnt;
}

template <typename T>
void ElementBuffer::update(const T vertices, std::size_t vertexCnt, GLenum type,
                           GLenum mode) {
    static_assert(std::is_pointer<T>::value, "Expected a pointer");
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertexCnt * sizeof(typename std::remove_pointer<T>::type),
                 vertices, mode);
    m_primitiveType = type;
    m_mode = mode;
    m_attrFunction = &std::remove_pointer<T>::type::setupAttribute;
    m_attrFunction();
}

#endif
