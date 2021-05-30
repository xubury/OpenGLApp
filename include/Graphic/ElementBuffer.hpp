#ifndef VERTEX_ELEMENT_HPP
#define VERTEX_ELEMENT_HPP

#include <Graphic/BufferObject.hpp>

class GRAPHIC_API ElementBuffer : public BufferObject {
   public:
    ElementBuffer();

    ~ElementBuffer();

    ElementBuffer(const ElementBuffer &other);

    ElementBuffer(ElementBuffer &&other);

    ElementBuffer &operator=(ElementBuffer other);

    friend void swap(ElementBuffer &first, ElementBuffer &second);

    bool initialize() override;

    template <typename T>
    void update(const T vertices, std::size_t vertexCnt,
                const uint32_t *indices, std::size_t indexCnt, GLenum type,
                GLenum mode);

    template <typename T>
    void update(const T vertices, std::size_t vertexCnt, GLenum type,
                GLenum mode);

    void drawPrimitive() const override;

   private:
    uint32_t m_VBO;
    uint32_t m_EBO;
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
    bindVertexArray();
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
